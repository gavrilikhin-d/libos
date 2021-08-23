// Keyboard I/O manipulations

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

/** @file os/keyboard.hpp
 *  Keyboard I/O manipulations
 */

#pragma once

#include <vector>

#include "os/macros.h"

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

    /* Letters */
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

    /* Other symbols */
    Space = ' ',
// TODO:
//  '`' / '~'
//  '-' / '_'
//  '=' / '+'
//  '\' / '|'
//  '[' / '{'
//  ']' / '}'
//  ';' / ':'
//  ''' / '"'
//  ',' / '<'
//  '.' / '>'
//  '/' / '?'

    /* Modifiers */
#if OS_LINUX
    Shift_L   = 0xFFE1,
    Shift_R   = 0xFFE2,
    Control_L = 0xFFE3,
    Control_R = 0xFFE4,
    Caps_Lock = 0xFFE5,
    Alt_L     = 0xFFE9,
    Alt_R     = 0xFFEA,
    Super_L   = 0xFFEB,
    Super_R   = 0xFFEC,
#elif OS_WINDOWS
    Shift_L   = 0xA0,
    Shift_R   = 0xA1,
    Control_L = 0xA2,
    Control_R = 0xA3,
    Caps_Lock = 0x14,
    Alt_L     = 0xA4,
    Alt_R     = 0xA5,
    Super_L   = 0x5B,
    Super_R   = 0x5C,
#endif
    Shift = Shift_L,
    Control = Control_L, Ctrl = Control_L,
    Caps = Caps_Lock,
    Alt = Alt_L,
    Super = Super_L, Win = Super_L, Command = Super_L, Cmd = Super_L,

    /* TTY function keys */
#if IS_OS_LINUX
    BackSpace = 0xFF08,
    Tab       = 0xFF09,
    Return    = 0xFF0d,
    Escape    = 0xFF1B,
    Delete    = 0xFFFF,
#elif IS_OS_WINDOWS
    BackSpace = 0x08,
    Tab       = '\t',
    Return    = '\r',
    Escape    = 0x1B,
    Delete    = 0x2E,
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
#endif

    /* Numpad */
#if IS_OS_LINUX
    num_0 = 0xFFB0,
    num_1 = 0xFFB1,
    num_2 = 0xFFB2,
    num_3 = 0xFFB3,
    num_4 = 0xFFB4,
    num_5 = 0xFFB5,
    num_6 = 0xFFB6,
    num_7 = 0xFFB7,
    num_8 = 0xFFB8,
    num_9 = 0xFFB9
#elif IS_OS_WINDOWS
    num_0 = 0x60,
    num_1 = 0x61,
    num_2 = 0x62,
    num_3 = 0x63,
    num_4 = 0x64,
    num_5 = 0x65,
    num_6 = 0x66,
    num_7 = 0x67,
    num_8 = 0x68,
    num_9 = 0x69
#endif

// TODO: more numpad keys

// TODO: PrintScreen, Insert, etc...
};

/// Combination of keys
struct combination
{
    /// Array of virtual keys
    std::vector<vk> keys;

    /// Make combination from virtual keys
    combination(vk key) : keys({key}) {}
    /// Make combination from list of virtual keys
    combination(std::initializer_list<vk> keys = {}) : keys(keys) {}

    /// Append a combination
    combination & operator+=(const combination &combo)
    {
        keys.insert(keys.end(), combo.keys.begin(), combo.keys.end());
        return *this;
    }

    /// Concatinate 2 combinations
    combination operator+(const combination &combo) const { return combination{*this} += combo; }
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