#include "os/keyboard.hpp"

#include "os/macros.h"
#if !IS_OS_MACOS
    #error "This code is for macOS only!"
#endif

#include <Carbon/Carbon.h>

namespace os::detail
{

CGEventFlags extract_modifiers(os::keyboard::combination &combo)
{
    using os::keyboard::vk;

    CGEventFlags flags = 0;

    auto try_extract = [&flags, &combo](vk m1, vk m2, CGEventFlags mask)
    {
        if (combo.keys.contains(m1) || combo.keys.contains(m2))
        {
            flags |= mask;
            combo.keys.erase(m1);
            combo.keys.erase(m2);
        }
    };
    try_extract(vk::Shift_L, vk::Shift_R, kCGEventFlagMaskShift);
    try_extract(vk::Option_L, vk::Option_R, kCGEventFlagMaskAlternate);
    try_extract(vk::Command_L, vk::Command_R, kCGEventFlagMaskCommand);
    try_extract(vk::Control_L, vk::Control_R, kCGEventFlagMaskControl);
    // Doesn't have right equivalent
    try_extract(vk::Function, vk::Function, kCGEventFlagMaskSecondaryFn);

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

} // namespace os::detail


namespace os::keyboard
{

// Check if every key in combination is pressed
bool is_pressed(const combination &combo)
{
    return true;
}

// Get combination of all pressed keys on a keyboard
combination pressed_keys()
{
    combination combo;


    return combo;
}

// Press combination of keys (until release)
void press(const combination &combo) { detail::send_key_events(combo, true); }

// Release combination of keys
void release(const combination &combo) { detail::send_key_events(combo, false); }

} // namespace os::keyboard