// Functions to get OS Kernel info

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

/** @file os/kernel.hpp
 *  Functions to get OS Kernel info
 */

#pragma once

#include <string>

#include "os/version.hpp"

namespace os::kernel
{

/**
 * @brief Get OS Kernel name
 *
 * @returns
 *  - Linux: `"Linux"`
 *  - MacOS: `"Darwin"`
 *  - Windows: `"Windows NT"`
 */
std::string name();

/// Get OS Kernel major, minor and patch version as integers
::version version();

/// Get OS Kernel version as string
std::string version_string();

/// Full OS Kernel info
struct info_t
{
    /// OS Kernel name
    std::string name;
    /// OS Kernel major, minor and patch version as integers
    ::version   version;
    /// OS Kernel version as string
    std::string version_string;
};

/**
 * @brief Get full OS Kernel info
 * @details
 *  Obtaining OS Kernel info is very expensive.
 *  Hence, it's statically allocated and read exactly once.
 *
 * @return const info_t& Ref to OS Kernel info
 */
const info_t & info();

} // namespace os::kernel