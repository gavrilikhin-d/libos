#include "os/keyboard.hpp"

#include "os/macros.h"
#if !IS_OS_MACOS
    #error "This code is for macOS only!"
#endif

#include <algorithm>
#include <string>
#include <unordered_map>

#include <Carbon/Carbon.h>
#include <IOKit/hid/IOHIDDevice.h>
#include <IOKit/hid/IOHIDManager.h>

namespace os::detail
{

CGEventFlags extract_modifiers(os::keyboard::combination &combo)
{
    using os::keyboard::vk;

    CGEventFlags flags = 0;

    auto try_extract = [&flags, &combo](vk modifier, CGEventFlags mask)
    {
        auto it = combo.keys.find(modifier); 
        if (it != combo.keys.end())
        {
            flags |= mask;
            combo.keys.erase(it);
        }
    };

    try_extract(vk::Function, kCGEventFlagMaskSecondaryFn);

    try_extract(vk::Shift_L, kCGEventFlagMaskShift);
    try_extract(vk::Shift_R, kCGEventFlagMaskShift);
    try_extract(vk::Option_L, kCGEventFlagMaskAlternate);
    try_extract(vk::Option_R, kCGEventFlagMaskAlternate);
    try_extract(vk::Command_L, kCGEventFlagMaskCommand);
    try_extract(vk::Command_R, kCGEventFlagMaskCommand);
    try_extract(vk::Control_L, kCGEventFlagMaskControl);
    try_extract(vk::Control_R, kCGEventFlagMaskControl);

    return flags;
}

void send_key_events(const os::keyboard::combination &combo, bool is_down)
{
    os::keyboard::combination no_modifiers = combo;
    CGEventFlags flags = extract_modifiers(no_modifiers);

    for (auto key : no_modifiers.keys)
    {
        CGEventRef event = CGEventCreateKeyboardEvent(
            nullptr, 
            static_cast<CGKeyCode>(key), 
            is_down
        );
        CGEventSetFlags(event, flags);
        CGEventPost(kCGHIDEventTap, event);
        CFRelease(event);
    }
}

class HIDInputManager
{
public:
    static HIDInputManager & get() 
    {
        static HIDInputManager input; 
        return input;
    }

    bool is_pressed(const os::keyboard::combination &combo) const
    {
        return std::all_of(
            combo.keys.begin(), combo.keys.end(), 
            [this](const auto &vk)
            {
                IOHIDValueRef value = 0;

                IOHIDDeviceRef device = IOHIDElementGetDevice(keys.at(vk));
                IOHIDDeviceGetValue(device, keys.at(vk), &value);

                return IOHIDValueGetIntegerValue(value) == 1;
            }
        );
    }

    os::keyboard::combination pressed_keys() const
    {
        keyboard::combination combo;
        for (const auto &[vk, key] : keys)
        {
            if (is_pressed(vk)) 
            {
                combo += vk;
            }
        }
        return combo;
    }

private:
    CFDataRef               layout_data; // Layout data
    const UCKeyboardLayout *layout;      // Current keyboard layout
    IOHIDManagerRef         manager;                    

    std::unordered_map<keyboard::vk, IOHIDElementRef> keys;

    HIDInputManager() 
    {
        // Get the current keyboard layout
        TISInputSourceRef tis = TISCopyCurrentKeyboardLayoutInputSource();
        layout_data = static_cast<CFDataRef>(
            TISGetInputSourceProperty(tis, kTISPropertyUnicodeKeyLayoutData)
        );

        if (!layout_data) { return; }

        // Keep a reference for ourself
        CFRetain(layout_data);
        // The TIS is no more needed
        CFRelease(tis);

        layout = reinterpret_cast<const UCKeyboardLayout *>(CFDataGetBytePtr(layout_data));

        // Create an HID Manager reference
        manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
        // Open the HID Manager reference
        IOReturn openStatus = IOHIDManagerOpen(manager, kIOHIDOptionsTypeNone);

        if (openStatus != kIOReturnSuccess) { return; }

        // Initialize the keyboard
        init_keyboard();
    }

    CFDictionaryRef copy_devices_mask(UInt32 page, UInt32 usage)
    {
        // Create the dictionary.
        CFMutableDictionaryRef dict = 
            CFDictionaryCreateMutable(
                kCFAllocatorDefault, 
                2, // capacity
                &kCFTypeDictionaryKeyCallBacks,
                &kCFTypeDictionaryValueCallBacks
            );

        // Add the page value.
        CFNumberRef value = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &page);
        CFDictionarySetValue(dict, CFSTR(kIOHIDDeviceUsagePageKey), value);
        CFRelease(value);

        // Add the usage value (which is only valid if page value exists).
        value = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage);
        CFDictionarySetValue(dict, CFSTR(kIOHIDDeviceUsageKey), value);
        CFRelease(value);

        return dict;
    }

    CFSetRef copy_devices(UInt32 page, UInt32 usage)
    {
        // Filter and keep only the requested devices
        CFDictionaryRef mask = copy_devices_mask(page, usage);

        IOHIDManagerSetDeviceMatching(manager, mask);

        CFRelease(mask);
        mask = nullptr;

        CFSetRef devices = IOHIDManagerCopyDevices(manager);
        if (devices == nullptr) { return nullptr; }

        // Is there at least one device?
        CFIndex device_count = CFSetGetCount(devices);
        if (device_count == 0)
        {
            CFRelease(devices);
            return nullptr;
        }

        return devices;
    }

    void init_keyboard()
    {
        ////////////////////////////////////////////////////////////
        // The purpose of this function is to initialize keys so we can get
        // the associate IOHIDElementRef with a vk in ~constant~ time.

        // Get only keyboards
        CFSetRef keyboards = copy_devices(kHIDPage_GenericDesktop, kHIDUsage_GD_Keyboard);
        if (keyboards == nullptr) { return; }

        CFIndex keyboard_count = CFSetGetCount(keyboards); // >= 1 (asserted by copyDevices)

        // Get an iterable array
        CFTypeRef devices_array[keyboard_count];
        CFSetGetValues(keyboards, devices_array);

        for (CFIndex i = 0; i < keyboard_count; ++i)
        {
            IOHIDDeviceRef keyboard = static_cast<IOHIDDeviceRef>(
                const_cast<void *>(devices_array[i])
            );
            load_keyboard(keyboard);
        }

        // Release unused stuff
        CFRelease(keyboards);
    }


    void load_keyboard(IOHIDDeviceRef keyboard)
    {
        CFArrayRef keys = 
            IOHIDDeviceCopyMatchingElements(
                keyboard,
                nullptr, // match all
                kIOHIDOptionsTypeNone
            );
        if (keys == nullptr) { return; }

        // How many elements are there?
        CFIndex keys_count = CFArrayGetCount(keys);
        if (keys_count == 0) { return; }

        // Go through all connected elements.
        for (CFIndex i = 0; i < keys_count; ++i)
        {
            IOHIDElementRef key = static_cast<IOHIDElementRef>(
                const_cast<void *>(CFArrayGetValueAtIndex(keys, i))
            );
            // Skip non-matching keys elements
            if (IOHIDElementGetUsagePage(key) != kHIDPage_KeyboardOrKeypad) { continue; }
            load_key(key);
        }

        // Release unused stuff
        CFRelease(keys);
    }

    void load_key(IOHIDElementRef key)
    {
        // Get its virtual code
        UInt32 usage = IOHIDElementGetUsage(key);
        UInt8  virtual_code = usage_to_virtual_code(usage);

        if (virtual_code == 0xff) { return; }

        // Now translate the virtual code to Unicode according to
        // the current keyboard layout

        UInt32       deadKeyState = 0;
        // Unicode string length is usually less or equal to 4
        UniCharCount maxStringLength = 4;
        UniCharCount actualStringLength = 0;
        UniChar      unicodeString[maxStringLength];

        OSStatus     error;

        error = 
            UCKeyTranslate(
                layout,                      // current layout
                virtual_code,                // our key
                kUCKeyActionDown,            // or kUCKeyActionUp ?
                0x100,                       // no modifiers
                LMGetKbdType(),              // keyboard's type
                kUCKeyTranslateNoDeadKeysBit,// some sort of option
                &deadKeyState,               // unused stuff
                maxStringLength,             // our memory limit
                &actualStringLength,         // length of what we get
                unicodeString
            ); 

        if (error == noErr)
        {
            // Translation went fine


            bool found = false;
            keyboard::vk vk;

            // First we look if the key down is from a list of characters
            // that depend on keyboard localization
            if (actualStringLength > 0)
            {
                found = localizedKeys(unicodeString[0], vk);
            }

            // The key is not a localized one so we try to find a
            // corresponding code through virtual key code
            if (not found)
            {
                vk = static_cast<keyboard::vk>(virtual_code);
            }

            // Ok, everything went fine. Now we have a unique
            // corresponding sf::Keyboard::Key to one IOHIDElementRef
            keys[vk] = key;

            // And don't forget to keep the reference alive for our usage
            CFRetain(keys[vk]);
        }
    }

    UInt8 usage_to_virtual_code(UInt32 usage)
    {
        switch (usage)
        {
            case kHIDUsage_KeyboardA:                   return 0x00;
            case kHIDUsage_KeyboardB:                   return 0x0B;
            case kHIDUsage_KeyboardC:                   return 0x08;
            case kHIDUsage_KeyboardD:                   return 0x02;
            case kHIDUsage_KeyboardE:                   return 0x0e;
            case kHIDUsage_KeyboardF:                   return 0x03;
            case kHIDUsage_KeyboardG:                   return 0x05;
            case kHIDUsage_KeyboardH:                   return 0x04;
            case kHIDUsage_KeyboardI:                   return 0x22;
            case kHIDUsage_KeyboardJ:                   return 0x26;
            case kHIDUsage_KeyboardK:                   return 0x28;
            case kHIDUsage_KeyboardL:                   return 0x25;
            case kHIDUsage_KeyboardM:                   return 0x2e;
            case kHIDUsage_KeyboardN:                   return 0x2d;
            case kHIDUsage_KeyboardO:                   return 0x1f;
            case kHIDUsage_KeyboardP:                   return 0x23;
            case kHIDUsage_KeyboardQ:                   return 0x0c;
            case kHIDUsage_KeyboardR:                   return 0x0f;
            case kHIDUsage_KeyboardS:                   return 0x01;
            case kHIDUsage_KeyboardT:                   return 0x11;
            case kHIDUsage_KeyboardU:                   return 0x20;
            case kHIDUsage_KeyboardV:                   return 0x09;
            case kHIDUsage_KeyboardW:                   return 0x0d;
            case kHIDUsage_KeyboardX:                   return 0x07;
            case kHIDUsage_KeyboardY:                   return 0x10;
            case kHIDUsage_KeyboardZ:                   return 0x06;

            case kHIDUsage_Keyboard1:                   return 0x12;
            case kHIDUsage_Keyboard2:                   return 0x13;
            case kHIDUsage_Keyboard3:                   return 0x14;
            case kHIDUsage_Keyboard4:                   return 0x15;
            case kHIDUsage_Keyboard5:                   return 0x17;
            case kHIDUsage_Keyboard6:                   return 0x16;
            case kHIDUsage_Keyboard7:                   return 0x1a;
            case kHIDUsage_Keyboard8:                   return 0x1c;
            case kHIDUsage_Keyboard9:                   return 0x19;
            case kHIDUsage_Keyboard0:                   return 0x1d;

            case kHIDUsage_KeyboardReturnOrEnter:       return 0x24;
            case kHIDUsage_KeyboardEscape:              return 0x35;
            case kHIDUsage_KeyboardDeleteOrBackspace:   return 0x33;
            case kHIDUsage_KeyboardTab:                 return 0x30;
            case kHIDUsage_KeyboardSpacebar:            return 0x31;
            case kHIDUsage_KeyboardHyphen:              return 0x1b;
            case kHIDUsage_KeyboardEqualSign:           return 0x18;
            case kHIDUsage_KeyboardOpenBracket:         return 0x21;
            case kHIDUsage_KeyboardCloseBracket:        return 0x1e;
            case kHIDUsage_KeyboardBackslash:           return 0x2a;
            case kHIDUsage_KeyboardSemicolon:           return 0x29;
            case kHIDUsage_KeyboardQuote:               return 0x27;
            case kHIDUsage_KeyboardGraveAccentAndTilde: return 0x32;
            case kHIDUsage_KeyboardComma:               return 0x2b;
            case kHIDUsage_KeyboardPeriod:              return 0x2F;
            case kHIDUsage_KeyboardSlash:               return 0x2c;
            case kHIDUsage_KeyboardCapsLock:            return 0x39;

            case kHIDUsage_KeyboardF1:                  return 0x7a;
            case kHIDUsage_KeyboardF2:                  return 0x78;
            case kHIDUsage_KeyboardF3:                  return 0x63;
            case kHIDUsage_KeyboardF4:                  return 0x76;
            case kHIDUsage_KeyboardF5:                  return 0x60;
            case kHIDUsage_KeyboardF6:                  return 0x61;
            case kHIDUsage_KeyboardF7:                  return 0x62;
            case kHIDUsage_KeyboardF8:                  return 0x64;
            case kHIDUsage_KeyboardF9:                  return 0x65;
            case kHIDUsage_KeyboardF10:                 return 0x6d;
            case kHIDUsage_KeyboardF11:                 return 0x67;
            case kHIDUsage_KeyboardF12:                 return 0x6f;

            case kHIDUsage_KeyboardInsert:              return 0x72;
            case kHIDUsage_KeyboardHome:                return 0x73;
            case kHIDUsage_KeyboardPageUp:              return 0x74;
            case kHIDUsage_KeyboardDeleteForward:       return 0x75;
            case kHIDUsage_KeyboardEnd:                 return 0x77;
            case kHIDUsage_KeyboardPageDown:            return 0x79;

            case kHIDUsage_KeyboardRightArrow:          return 0x7c;
            case kHIDUsage_KeyboardLeftArrow:           return 0x7b;
            case kHIDUsage_KeyboardDownArrow:           return 0x7d;
            case kHIDUsage_KeyboardUpArrow:             return 0x7e;

            case kHIDUsage_KeypadNumLock:               return 0x47;
            case kHIDUsage_KeypadSlash:                 return 0x4b;
            case kHIDUsage_KeypadAsterisk:              return 0x43;
            case kHIDUsage_KeypadHyphen:                return 0x4e;
            case kHIDUsage_KeypadPlus:                  return 0x45;
            case kHIDUsage_KeypadEnter:                 return 0x4c;

            case kHIDUsage_Keypad1:                     return 0x53;
            case kHIDUsage_Keypad2:                     return 0x54;
            case kHIDUsage_Keypad3:                     return 0x55;
            case kHIDUsage_Keypad4:                     return 0x56;
            case kHIDUsage_Keypad5:                     return 0x57;
            case kHIDUsage_Keypad6:                     return 0x58;
            case kHIDUsage_Keypad7:                     return 0x59;
            case kHIDUsage_Keypad8:                     return 0x5b;
            case kHIDUsage_Keypad9:                     return 0x5c;
            case kHIDUsage_Keypad0:                     return 0x52;

            case kHIDUsage_KeypadPeriod:                return 0x41;
            case kHIDUsage_KeyboardApplication:         return 0x6e;
            case kHIDUsage_KeypadEqualSign:             return 0x51;

            case kHIDUsage_KeyboardLeftControl:         return 0x3b;
            case kHIDUsage_KeyboardLeftShift:           return 0x38;
            case kHIDUsage_KeyboardLeftAlt:             return 0x3a;
            case kHIDUsage_KeyboardLeftGUI:             return 0x37;
            case kHIDUsage_KeyboardRightControl:        return 0x3e;
            case kHIDUsage_KeyboardRightShift:          return 0x3c;
            case kHIDUsage_KeyboardRightAlt:            return 0x3d;
            case kHIDUsage_KeyboardRightGUI:            return 0x36;

            default:                                    return 0xff;
        }
    }

    bool localizedKeys(UniChar c, keyboard::vk &vk)
    {
        if ('a' <= c && c <= 'z')
        {
            c = std::toupper(c);
        }

        #define CASE_LETTER(LETTER)        \
            case #LETTER[0]:               \
                vk = keyboard::vk::LETTER; \
                break; 

        switch (c)
        {
            CASE_LETTER(A);
            CASE_LETTER(B);
            CASE_LETTER(C);
            CASE_LETTER(D);
            CASE_LETTER(E);
            CASE_LETTER(F);
            CASE_LETTER(G);
            CASE_LETTER(H);
            CASE_LETTER(I);
            CASE_LETTER(J);
            CASE_LETTER(K);
            CASE_LETTER(L);
            CASE_LETTER(M);
            CASE_LETTER(N);
            CASE_LETTER(O);
            CASE_LETTER(P);
            CASE_LETTER(Q);
            CASE_LETTER(R);
            CASE_LETTER(S);
            CASE_LETTER(T);
            CASE_LETTER(U);
            CASE_LETTER(V);
            CASE_LETTER(W);
            CASE_LETTER(X);
            CASE_LETTER(Y);
            CASE_LETTER(Z);

            default: return false;
        }

        return true;
    }

    ~HIDInputManager()
    {
        if (layout_data)
        {
            CFRelease(layout_data);
        }

        // Do not release m_layout! It is owned by m_layoutData.

        if (manager)
        {
            CFRelease(manager);
        }

        for (auto &[vk, key] : keys)
        {
            CFRelease(key);
        }
    }
};

} // namespace os::detail


namespace os::keyboard
{

// Check if every key in combination is pressed
bool is_pressed(const combination &combo) { return detail::HIDInputManager::get().is_pressed(combo); }

// Get combination of all pressed keys on a keyboard
combination pressed_keys() { return detail::HIDInputManager::get().pressed_keys(); }

// Press combination of keys (until release)
void press(const combination &combo) { detail::send_key_events(combo, true); }

// Release combination of keys
void release(const combination &combo) { detail::send_key_events(combo, false); }

} // namespace os::keyboard