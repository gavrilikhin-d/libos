// Mouse input manipulations

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

/** @file os/mouse.hpp
 *  Mouse input manipulations
 */

#pragma once

/// TODO: move it somewhere else
/**
 * @brief
 *  Position on the screen.
 *  Uses `int` for negative values of relative coordinates.
 */
struct point { int x, y; };

namespace os::mouse
{

/// Mouse virtual buttons
enum class button
{
    left,
    /// Scroll wheel
    middle,
    right,

    _1,
    /// Backward navigation in browser
    backward = _1,
    _2,
    /// Forward navigation in browser
    forward = _2,
    _3,
    _4,
    _5
};


/// Check if button is pressed
bool is_pressed(button b);

/// Press mouse button (untill release())
void press(button b);
/// Release mouse button
void release(button b);

/// press() and release() mouse button
inline void click(button b) { press(b); release(b); }
/// Double click() mouse button
inline void double_click(button b) { click(b); click(b); }

/// Scrolling direction. Only used in scroll().
enum class scroll_direction { up, down };
/// Scroll wheel up or down
void scroll(scroll_direction d);
/// Check if wheel is scrolling at the moment
bool scrolling();

/// Get current cursor position
point position();
/// Move cursor to a new position
void move_to(point pos);


} // namespace os::mouse