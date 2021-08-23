// Functions to get OS info

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

/** @file os/info.hpp
 *  Functions to get OS info
 */

#pragma once

#include <string>

#include "os/macros.h"
#include "os/version.hpp"

// Protect from macro collision in std=gnu++17 extension
#undef linux

namespace os
{

/// Possible OS types
enum type_t
{
	undefined,

	linux,
	windows
};

/// Get type of OS at compile time
constexpr type_t type() noexcept
{
#if IS_OS_LINUX
    return os::linux;
#elif IS_OS_WINDOWS
    return os::windows;
#else
    return os::undefined;
#endif
}

/**
 * @brief Get OS name
 *
 * @returns
 *  - Linux:
 *      - Distributive's name (e.g. `"Ubuntu"`)
 *      - `"Linux"`, if `/etc/os-release` not found
 *  - Windows: `"Windows"`
 */
std::string name();

/// Get OS name with version
std::string pretty_name();

/**
 * @brief Get OS codename
 *
 * @returns
 *  - Linux: codename, if present
 *  - Windows: `""`
 */
std::string codename();

/// Get OS major, minor and patch version as integers
::version version();

/// Get OS version as string
std::string version_string();

/// Full OS info
struct info_t
{
    /// OS type
    type_t type = os::undefined;
    /// OS name
    std::string name;
    /// OS name with version
    std::string pretty_name;
    /// OS codename (if exists)
    std::string codename;
    /// OS major, minor and patch version as integers
    ::version   version;
    /// OS version as string
    std::string version_string;
};

/**
 * @brief Get full OS info
 * @details
 *  Obtaining OS info is very expensive.
 *  Hence, it's statically allocated and read exactly once.
 *
 * @return const info_t& Ref to OS info
 */
const info_t & info();

} // namespace os