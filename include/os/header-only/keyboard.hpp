// Keyboard I/O manipulations. Header-only

// This file is part of LibOS.

// Copyright (c) 2021 Gavrilikhin Daniil

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/** @file os/header-only/keyboard.hpp
 *  Keyboard I/O manipulations. Header-only
 */

#pragma once

#include <unordered_set>

// #include "os/macros.h"
// =========================

#if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
/// Defined if OS is Unix-like
#   define OS_UNIX 1
/// Set to 1 if OS is Unix-like, 0 otherwise
#   define IS_OS_UNIX 1
#else
#   define IS_OS_UNIX 0
#endif

#if defined(__linux__)
/// Defined if OS is Linux
#   define OS_LINUX 1
/// Set to 1 if OS is Linux, 0 otherwise
#	define IS_OS_LINUX 1
#else
#	define IS_OS_LINUX 0
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
/// Defined if OS is Windows
#   define OS_WINDOWS 1
/// Set to 1 if OS is Windows, 0 otherwise
#	define IS_OS_WINDOWS 1
#else
#	define IS_OS_WINDOWS 0
#endif

#if defined(__APPLE__) && defined(__MACH__)
/// Defined if OS is MacOS
#   define OS_MACOS 1
/// Set to 1 if OS is MacOS, 0 otherwise
#   define IS_OS_MACOS 1
#else
#   define IS_OS_MACOS 0
#endif
// End of   "os/macros.h"
// =========================


namespace os::keyboard
{

/**
 * @brief Virtual keys
 *
 * @details
 * Virtual-key code is a device-independent value
 * defined by the system that identifies the purpose of a key.
 * The output of sending a virtual key is affected by current keyboard layout and mapping.
 *
 * @note Some enum values are different on different OS.
 */
enum class vk
{
    /* Digits */
#if IS_OS_MACOS
    // From <Carbon/Carbon.h>
    Key_0 = 0x1D, // kVK_ANSI_0 = 0x1D,
    Key_1 = 0x12, // kVK_ANSI_1 = 0x12,
    Key_2 = 0x13, // kVK_ANSI_2 = 0x13,
    Key_3 = 0x14, // kVK_ANSI_3 = 0x14,
    Key_4 = 0x15, // kVK_ANSI_4 = 0x15,
    Key_5 = 0x17, // kVK_ANSI_5 = 0x17,
    Key_6 = 0x16, // kVK_ANSI_6 = 0x16,
    Key_7 = 0x1A, // kVK_ANSI_7 = 0x1A,
    Key_8 = 0x1C, // kVK_ANSI_8 = 0x1C,
    Key_9 = 0x19, // kVK_ANSI_9 = 0x19,
#else
    Key_0 = '0',
    Key_1 = '1',
    Key_2 = '2',
    Key_3 = '3',
    Key_4 = '4',
    Key_5 = '5',
    Key_6 = '6',
    Key_7 = '7',
    Key_8 = '8',
    Key_9 = '9',
#endif

    /* Letters */
#if IS_OS_MACOS
    A = 0x00, // kVK_ANSI_A = 0x00,
    B = 0x0B, // kVK_ANSI_B = 0x0B,
    C = 0x08, // kVK_ANSI_C = 0x08,
    D = 0x02, // kVK_ANSI_D = 0x02,
    E = 0x0E, // kVK_ANSI_E = 0x0E,
    F = 0x03, // kVK_ANSI_F = 0x03,
    G = 0x05, // kVK_ANSI_G = 0x05,
    H = 0x04, // kVK_ANSI_H = 0x04,
    I = 0x22, // kVK_ANSI_I = 0x22,
    J = 0x26, // kVK_ANSI_J = 0x26,
    K = 0x28, // kVK_ANSI_K = 0x28,
    L = 0x25, // kVK_ANSI_L = 0x25,
    M = 0x2E, // kVK_ANSI_M = 0x2E,
    N = 0x2D, // kVK_ANSI_N = 0x2D,
    O = 0x1F, // kVK_ANSI_O = 0x1F,
    P = 0x23, // kVK_ANSI_P = 0x23,
    Q = 0x0C, // kVK_ANSI_Q = 0x0C,
    R = 0x0F, // kVK_ANSI_R = 0x0F,
    S = 0x01, // kVK_ANSI_S = 0x01,
    T = 0x11, // kVK_ANSI_T = 0x11,
    U = 0x20, // kVK_ANSI_U = 0x20,
    V = 0x09, // kVK_ANSI_V = 0x09,
    W = 0x0D, // kVK_ANSI_W = 0x0D,
    X = 0x07, // kVK_ANSI_X = 0x07,
    Y = 0x10, // kVK_ANSI_Y = 0x10,
    Z = 0x06, // kVK_ANSI_Z = 0x06,
#else
    A = 'A',
    B = 'B',
    C = 'C',
    D = 'D',
    E = 'E',
    F = 'F',
    G = 'G',
    H = 'H',
    I = 'I',
    J = 'J',
    K = 'K',
    L = 'L',
    M = 'M',
    N = 'N',
    O = 'O',
    P = 'P',
    Q = 'Q',
    R = 'R',
    S = 'S',
    T = 'T',
    U = 'U',
    V = 'V',
    W = 'W',
    X = 'X',
    Y = 'Y',
    Z = 'Z',
#endif

    /* Other symbols */
#if IS_OS_MACOS
    Space     = 0x31, // kVK_Space = 0x31,

    Section   = 0x0A, // kVK_ISO_Section = 0x0A 

    Minus     = 0x1B, // kVK_ANSI_Minus        = 0x1B,
    Equal     = 0x18, // kVK_ANSI_Equal        = 0x18,
    Bracket_L = 0x21, // kVK_ANSI_LeftBracket  = 0x21,
    Bracket_R = 0x1E, // kVK_ANSI_RightBracket = 0x1E,
    Semicolon = 0x29, // kVK_ANSI_Semicolon    = 0x29,
    Quote     = 0x27, // kVK_ANSI_Quote        = 0x27,
    Backslash = 0x2A, // kVK_ANSI_Backslash    = 0x2A,
    Comma     = 0x2B, // kVK_ANSI_Comma        = 0x2B,
    Period    = 0x2F, // kVK_ANSI_Period       = 0x2F,
    Slash     = 0x2C, // kVK_ANSI_Slash        = 0x2C,
    Grave     = 0x32, // kVK_ANSI_Grave        = 0x32,  
#elif IS_OS_LINUX
    Space = ' ',
#elif IS_OS_WINDOWS
    Space = ' ',
#endif

    /* Modifiers */
#if IS_OS_LINUX
    Shift_L   = 0xFFE1,
    Shift_R   = 0xFFE2,
    Control_L = 0xFFE3,
    Control_R = 0xFFE4,
    Caps_Lock = 0xFFE5,
    Alt_L     = 0xFFE9,
    Alt_R     = 0xFFEA,
    Super_L   = 0xFFEB,
    Super_R   = 0xFFEC,

    Alt = Alt_L,
    Super = Super_L,
#elif IS_OS_WINDOWS
    Shift_L   = 0xA0,
    Shift_R   = 0xA1,
    Control_L = 0xA2,
    Control_R = 0xA3,
    Caps_Lock = 0x14,
    Alt_L     = 0xA4,
    Alt_R     = 0xA5,
    Win_L     = 0x5B,
    Win_R     = 0x5C,

    Alt = Alt_L,
    Win = Win_L,
#elif IS_OS_MACOS
    Shift_L   = 0x38, // kVK_Shift        = 0x38,
    Shift_R   = 0x3C, // kVK_RightShift   = 0x3C,
    Control_L = 0x3B, // kVK_Control      = 0x3B,
    Control_R = 0x3E, // kVK_RightControl = 0x3E,
    Caps_Lock = 0x39, // kVK_CapsLock     = 0x39,
    Option_L  = 0x3A, // kVK_Option       = 0x3A,
    Option_R  = 0x3D, // kVK_RightOption  = 0x3D,
    Command_L = 0x37, // kVK_Command      = 0x37,
    Command_R = 0x36, // kVK_RightCommand = 0x36,
    Function  = 0x3F, // kVK_Function     = 0x3F,

    Option = Option_L, Alt_L = Option_L, Alt_R = Option_R, Alt = Option_L,
    Command = Command_L, Cmd = Command_L,
    Fn = Function,
#endif
    Shift = Shift_L,
    Control = Control_L, Ctrl = Control_L,
    Caps = Caps_Lock,

    /* TTY function keys */
#if IS_OS_LINUX
    Backspace = 0xFF08,
    Tab       = 0xFF09,
    Return    = 0xFF0d,
    Escape    = 0xFF1B,
    Delete    = 0xFFFF,
#elif IS_OS_WINDOWS
    Backspace = 0x08,
    Tab       = '\t',
    Return    = '\r',
    Escape    = 0x1B,
    Delete    = 0x2E,
#elif IS_OS_MACOS
    Tab    = 0x30, // kVK_Tab    = 0x30,
    Return = 0x24, // kVK_Return = 0x24,
    Escape = 0x35, // kVK_Escape = 0x35,
    Delete = 0x33, // kVK_Delete = 0x33,
#endif
    Enter = Return,
    Esc = Escape,
    Del = Delete,

    /* Function keys */
#if IS_OS_LINUX
    F1  = 0xFFBE,
    F2  = 0xFFBF,
    F3  = 0xFFC0,
    F4  = 0xFFC1,
    F5  = 0xFFC2,
    F6  = 0xFFC3,
    F7  = 0xFFC4,
    F8  = 0xFFC5,
    F9  = 0xFFC6,
    F10 = 0xFFC7,
    F11 = 0xFFC8,
    F12 = 0xFFC9,
#elif IS_OS_WINDOWS
    F1  = 0x70,
    F2  = 0x71,
    F3  = 0x72,
    F4  = 0x73,
    F5  = 0x74,
    F6  = 0x75,
    F7  = 0x76,
    F8  = 0x77,
    F9  = 0x78,
    F10 = 0x79,
    F11 = 0x7A,
    F12 = 0x7B,
#elif IS_OS_MACOS
    F1  = 0x7A, // kVK_F1  = 0x7A,
    F2  = 0x78, // kVK_F2  = 0x78,
    F3  = 0x63, // kVK_F3  = 0x63,
    F4  = 0x76, // kVK_F4  = 0x76,
    F5  = 0x60, // kVK_F5  = 0x60,
    F6  = 0x61, // kVK_F6  = 0x61,
    F7  = 0x62, // kVK_F7  = 0x62,
    F8  = 0x64, // kVK_F8  = 0x64,
    F9  = 0x65, // kVK_F9  = 0x65,
    F10 = 0x6D, // kVK_F10 = 0x6D,
    F11 = 0x67, // kVK_F11 = 0x67,
    F12 = 0x6F, // kVK_F12 = 0x6F,
#endif

    /* Arrows */
#if IS_OS_LINUX
    Left  = 0xFF51,
    Up    = 0xFF52,
    Right = 0xFF53,
    Down  = 0xFF54,
#elif IS_OS_WINDOWS
    Left  = 0x25,
    Up    = 0x26,
    Right = 0x27,
    Down  = 0x28,
#elif IS_OS_MACOS
    Left  = 0x7B, // kVK_LeftArrow  = 0x7B,
    Up    = 0x7E, // kVK_UpArrow    = 0x7E, 
    Right = 0x7C, // kVK_RightArrow = 0x7C,
    Down  = 0x7D, // kVK_DownArrow  = 0x7D,
#endif

    /* Numpad */
#if IS_OS_LINUX
    Num_0 = 0xFFB0,
    Num_1 = 0xFFB1,
    Num_2 = 0xFFB2,
    Num_3 = 0xFFB3,
    Num_4 = 0xFFB4,
    Num_5 = 0xFFB5,
    Num_6 = 0xFFB6,
    Num_7 = 0xFFB7,
    Num_8 = 0xFFB8,
    Num_9 = 0xFFB9
#elif IS_OS_WINDOWS
    Num_0 = 0x60,
    Num_1 = 0x61,
    Num_2 = 0x62,
    Num_3 = 0x63,
    Num_4 = 0x64,
    Num_5 = 0x65,
    Num_6 = 0x66,
    Num_7 = 0x67,
    Num_8 = 0x68,
    Num_9 = 0x69
#elif IS_OS_MACOS
    Num_0 = 0x52, // kVK_ANSI_Keypad0 = 0x52,
    Num_1 = 0x53, // kVK_ANSI_Keypad1 = 0x53,
    Num_2 = 0x54, // kVK_ANSI_Keypad2 = 0x54,
    Num_3 = 0x55, // kVK_ANSI_Keypad3 = 0x55,
    Num_4 = 0x56, // kVK_ANSI_Keypad4 = 0x56,
    Num_5 = 0x57, // kVK_ANSI_Keypad5 = 0x57,
    Num_6 = 0x58, // kVK_ANSI_Keypad6 = 0x58,
    Num_7 = 0x59, // kVK_ANSI_Keypad7 = 0x59,
    Num_8 = 0x5B, // kVK_ANSI_Keypad8 = 0x5B,
    Num_9 = 0x5C, // kVK_ANSI_Keypad9 = 0x5C,

    Num_Decimal  = 0x41, // kVK_ANSI_KeypadDecimal  = 0x41,
    Num_Multiply = 0x43, // kVK_ANSI_KeypadMultiply = 0x43,
    Num_Plus     = 0x45, // kVK_ANSI_KeypadPlus     = 0x45,
    Num_Clear    = 0x47, // kVK_ANSI_KeypadClear    = 0x47,
    Num_Divide   = 0x4B, // kVK_ANSI_KeypadDivide   = 0x4B,
    Num_Minus    = 0x4E, // kVK_ANSI_KeypadMinus    = 0x4E,
    Num_Equals   = 0x51, // kVK_ANSI_KeypadEquals   = 0x51,
#endif

    /* Control keys */
#if IS_OS_MACOS
    Volume_Up   = 0x48, // kVK_VolumeUp   = 0x48,
    Volume_Down = 0x49, // kVK_VolumeDown = 0x49,
    Mute        = 0x4A, // kVK_Mute       = 0x4A,
    Help        = 0x72, // kVK_Help       = 0x72,
    Home        = 0x73, // kVK_Home       = 0x73,
    Page_Up     = 0x74, // kVK_PageUp     = 0x74,
    Page_Down   = 0x79, // kVK_PageDown   = 0x79,
    End         = 0x77  // kVK_End        = 0x77,
#endif
};

/// Combination of keys
struct combination
{
    /// Array of virtual keys
    std::unordered_set<vk> keys;

    /// Make combination from virtual keys
    combination(vk key) : keys({key}) {}
    /// Make combination from list of virtual keys
    combination(std::initializer_list<vk> keys = {}) : keys(keys) {}

    /// Append a combination
    combination & operator+=(const combination &combo)
    {
        keys.insert(combo.keys.begin(), combo.keys.end());
        return *this;
    }

    /// Concatinate 2 combinations
    combination operator+(const combination &combo) const
    {
        combination res = *this;
        return res += combo;
    }
};

/// Make a combination from 2 virtual keys
inline combination operator+(vk lhs, vk rhs) { return combination{{lhs, rhs}}; }

/// Check if every key in combination is pressed
bool is_pressed(const combination &combo);
/// Get combination of all pressed keys on a keyboard
combination pressed_keys();

/// Press combination of keys (until release())
void press(const combination &combo);
/// Release combination of keys
void release(const combination &combo);
/// press() and release() combination of keys
inline void click(const combination &combo) { press(combo); release(combo); }

} // namespace os::keyboard


// -------------------------
// |        SOURCES        |
// -------------------------

#if IS_OS_LINUX
// src/linux/keyboard.cpp
// =========================

#if !IS_OS_LINUX
    #error "This code is for Linux only!"
#endif

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

namespace os::detail
{

// RAII wrapper for X Server's Display
class display_handler
{
public:
    static display_handler & get()
    {
        static display_handler handler { XOpenDisplay(nullptr) };
        return handler;
    }

    Display * native() const { return display; }

    display_handler(const display_handler &) = delete;
    display_handler(display_handler &&) = delete;
    void operator=(const display_handler &) = delete;
    void operator=(display_handler &&) = delete;

    ~display_handler() { XCloseDisplay(display); }

private:
    display_handler(Display *display) : display(display) {}

    Display *display = nullptr;
};

} // namespace os::detail

namespace os::keyboard
{

// Check if every key in combination is pressed
bool is_pressed(const combination &combo)
{
    auto &&h = os::detail::display_handler::get();

    char keys_return[32];
    XQueryKeymap(h.native(), keys_return);

    for (const auto &key : combo.keys)
    {
        KeyCode kc = XKeysymToKeycode(h.native(), static_cast<KeySym>(key));
        // Key not pressed
        if (!(keys_return[kc / 8] & (1 << (kc % 8)))) { return false; }
    }

    return true;
}

// Get combination of all pressed keys on a keyboard
combination pressed_keys()
{
    combination combo;

    auto &&h = os::detail::display_handler::get();

    char keys_return[32];
    XQueryKeymap(h.native(), keys_return);

    /* Check all 256 virtual keys */
    for (size_t m = 0; m < 32; ++m)
    {
        // Check every bit
        for (size_t n = 0; n < 8; ++n)
        {
            // Key is pressed
            if (keys_return[m] & (1 << n)) { combo += static_cast<vk>(8*m + n); }
        }
    }

    return combo;
}

// Press combination of keys (until release)
void press(const combination &combo)
{
    auto &&h = os::detail::display_handler::get();

    for (const auto &key : combo.keys)
    {
        XTestFakeKeyEvent(
            h.native(), // Display *
            XKeysymToKeycode(
                h.native(), // Display *
                static_cast<KeySym>(key) // Our vk values same as KeySym for linux
            ),
            true, // is_press
            0     // delay
        );
    }
}

// Release combination of keys
void release(const combination &combo)
{
    auto &&h = os::detail::display_handler::get();

    for (const auto &key : combo.keys)
    {
        XTestFakeKeyEvent(
            h.native(), // Display *
            XKeysymToKeycode(
                h.native(), // Display *
                static_cast<KeySym>(key) // Our vk values same as KeySym for linux
            ),
            false, // is_press
            0     // delay
        );
    }
}

} // namespace os::keyboard
// End of src/linux/keyboard.cpp
// =========================

#endif // IS_OS_LINUX

#if IS_OS_WINDOWS
// src/windows/keyboard.cpp
// =========================

#if !IS_OS_WINDOWS
    #error "This code is for Windows only!"
#endif

#include <Windows.h>


namespace os::keyboard
{

    // Check if every key in combination is pressed
    bool is_pressed(const combination& combo)
    {
        for (const auto& key : combo.keys)
        {
            short state = GetAsyncKeyState(static_cast<int>(key));
            // If the most significant bit of 2 bytes is not set, the key isn't pressed
            if (!(state & (1 << 15))) { return false; };
        }
        return true;
    }

    // Get combination of all pressed keys on a keyboard
    combination pressed_keys()
    {
        combination combo;

        // Go through every virtual key
        for (int key = 0; key < 256; ++key)
        {
            short state = GetAsyncKeyState(key);
            // If the most significant bit of 2 bytes set, the key is pressed
            if (state & (1 << 15)) { combo += static_cast<vk>(key); };
        }

        return combo;
    }

    // Press combination of keys (until release)
    void press(const combination& combo)
    {
        std::vector<INPUT> inputs(combo.keys.size());
        for (size_t i = 0; i < combo.keys.size(); ++i)
        {
            auto& in = inputs[i];
            const auto& key = combo.keys[i];

            in.type = INPUT_KEYBOARD;
            in.ki.wVk = static_cast<int>(key);
        }
        SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
    }

    // Release combination of keys
    void release(const combination& combo)
    {
        std::vector<INPUT> inputs(combo.keys.size());
        for (size_t i = 0; i < combo.keys.size(); ++i)
        {
            auto& in = inputs[i];
            const auto& key = combo.keys[i];

            in.type = INPUT_KEYBOARD;
            in.ki.wVk = static_cast<int>(key);
            in.ki.dwFlags = KEYEVENTF_KEYUP;
        }
        SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
    }

} // namespace os::keyboard
// End of src/windows/keyboard.cpp
// =========================

#endif // IS_OS_WINDOWS
