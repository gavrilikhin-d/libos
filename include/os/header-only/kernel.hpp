// Functions to get OS Kernel info. Header-only

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

/** @file os/header-only/kernel.hpp
 *  Functions to get OS Kernel info. Header-only
 */

#pragma once

#include <string>

// #include "os/version.hpp"
// =========================
#include <string>
#include <string_view>

/// Struct to represent semantic versioning
struct version
{
    /// Major version
    unsigned major = 0;
    /// Minor version
    unsigned minor = 0;
    /// Patch or build version
    unsigned patch = 0;

    /// Construct version from major, minor and patch numbers
    constexpr version(unsigned major = 0, unsigned minor = 0, unsigned patch = 0) noexcept
        : major(major), minor(minor), patch(patch) {}

    /**
     * @brief Construct version from string
     *
     * @param str String with format "major.minor.patch"
     *
     * @note
     *  If string format is wrong, no exception is thrown.
     *  All successfully read values will be saved.
     */
    explicit constexpr version(std::string_view str) noexcept
    {
        auto it = str.begin();
        unsigned* fields[3] = { &major, &minor, &patch };
        for (auto field : fields)
        {
            while (it != str.end() && '0' <= *it && *it <= '9')
            {
                *field *= 10;
                *field += *it - '0';
                it++;
            }

            // EOF or wrong delim
            if (it == str.end() || *it != '.') { break; }

            it++; // Skip delim
        }
    }

    /// Check version for equality
    constexpr bool operator==(const version &rhs) const noexcept
    {
        return (major == rhs.major) && (minor == rhs.minor) && (patch == rhs.patch);
    }
    /// Check if version is less
    constexpr bool operator< (const version &rhs) const noexcept
    {
        if (major < rhs.major) { return true; }
        if (minor < rhs.minor) { return true; }
        if (patch < rhs.patch) { return true; }
        return false;
    }
    /// Check if version is less or equal
    constexpr bool operator<=(const version &rhs) const noexcept
    {
        return (*this < rhs) || (*this == rhs);
    }
    /// Check if version is greater
    constexpr bool operator> (const version &rhs) const noexcept { return !(*this <= rhs); }
    /// Check if version is greater or equal
    constexpr bool operator>=(const version &rhs) const noexcept { return !(*this < rhs); }
    /// Check if version is not equal
    constexpr bool operator!=(const version &rhs) const noexcept { return !(*this == rhs); }


    /// Get version string in format "major.minor.patch"
    std::string str() const
    {
        return std::to_string(major) + "." +
               std::to_string(minor) + "." +
               std::to_string(patch);
    }
};
// End of   "os/version.hpp"
// =========================


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

// -------------------------
// |        SOURCES        |
// -------------------------

#if IS_OS_LINUX
// src/linux/kernel.cpp
// =========================

#if !IS_OS_LINUX
    #error "This code is for Linux only!"
#endif

#include <sys/utsname.h>

namespace os::kernel
{

// Get name of OS kernel
std::string name() { return "Linux"; }

// Get major, minor and patch of OS kernel
::version version() { return info().version; }

// Get version [+ additional data] of OS kernel
std::string version_string() { return info().version_string; }

// Get OS kernel info
const info_t & info()
{
    static info_t i;

    if (static bool init = true; init)
    {
        utsname utsname; uname(&utsname);
        i.name = os::kernel::name();
        i.version = ::version{utsname.release};
        i.version_string = utsname.release;

        init = false;
    }

    return i;
}

} // namespace os::kernel
// End of src/linux/kernel.cpp
// =========================

#endif // IS_OS_LINUX

#if IS_OS_WINDOWS
// src/windows/kernel.cpp
// =========================

#if !IS_OS_WINDOWS
    #error "This code is for Windows only!"
#endif

namespace os::kernel
{

// Get name of OS kernel
std::string name() { return "Windows NT"; }

// Get major, minor and patch of OS kernel
::version version() { return info().version; }

// Get version [+ additional data] of OS kernel
std::string version_string() { return info().version_string; }

// Get OS kernel info
const info_t& info()
{
    static info_t i;

    if (static bool init = true; init)
    {
        i.name = name();

        // KUSER_SHARED_DATA address.
        // Offsets are taken from http://terminus.rewolf.pl/terminus/structures/ntdll/_KUSER_SHARED_DATA_x64.html
        constexpr uintptr_t data_adress = uintptr_t{ 0x7ffe0000 };
        const uint32_t major = *reinterpret_cast<const uint32_t*>(data_adress + 0x26c);
        const uint32_t minor = *reinterpret_cast<const uint32_t*>(data_adress + 0x270);
        const uint32_t patch = *reinterpret_cast<const uint32_t*>(data_adress + 0x260);

        i.version = ::version{ major, minor, patch };
        i.version_string = i.version.str();

        init = false;
    }

    return i;
}

} // namespace os::kernel
// End of src/windows/kernel.cpp
// =========================

#endif // IS_OS_WINDOWS
