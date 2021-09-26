// Functions to get OS info. Header-only

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

/** @file os/header-only/info.hpp
 *  Functions to get OS info. Header-only
 */

#pragma once

#include <string>

// #include "os/macros.h"
// =========================

#if defined(__unix__)
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
// End of   "os/macros.h"
// =========================

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

// -------------------------
// |        SOURCES        |
// -------------------------

#if IS_OS_LINUX
// src/linux/info.cpp
// =========================

#if !IS_OS_LINUX
    #error "This code is for Linux only!"
#endif

#include <fstream>

#include <sys/utsname.h>
namespace os
{

// Name of OS without version
std::string name() { return info().name; }

// Name of OS + version
std::string pretty_name() { return info().pretty_name; }

// Codename of OS (if present)
std::string codename() { return info().codename; }

// Major, minor and patch of OS
::version version() { return info().version; }

// Version [+ some additional data]
std::string version_string() { return info().version_string; }

// Get whole OS info
const info_t & info()
{
    static info_t i;

    // Reading from file is expensive.
    // Init info only once.
    if (static bool init = true; init)
    {
        i.type = type();

        std::ifstream os_release("/etc/os-release");
        if (os_release)
        {
            std::string key, value;
            auto get_entry = [&os_release, &key, &value]() -> bool
            {
                std::getline(os_release, key, '=');
                std::getline(os_release, value, '\n');
                return os_release.good();
            };
            while (get_entry())
            {
                if (key == "NAME")
                {
                    // Remove ""
                    i.name = value.substr(1, value.size() - 2);
                }
                else if (key == "VERSION")
                {
                    // Remove ""
                    i.version_string = value.substr(1, value.size() - 2);
                }
                else if (key == "PRETTY_NAME")
                {
                    // Remove ""
                    i.pretty_name = value.substr(1, value.size() - 2);
                }
                else if (key == "VERSION_ID")
                {
                    // Remove ""
                    i.version = ::version{value.substr(1, value.size() - 2)};
                }
                else if (key == "VERSION_CODENAME")
                {
                    i.codename = value;
                }
            }
        }
        else
        {
            utsname utsname; uname(&utsname);
            i.name = "Linux";
            i.version = ::version{utsname.release};
            i.version_string = utsname.release;

            i.codename = "";
            i.pretty_name = i.name + " " + i.version_string;
        }

        init = false;
    }

    return i;
}

} // namespace os
// End of src/linux/info.cpp
// =========================

#endif // IS_OS_LINUX

#if IS_OS_WINDOWS
// src/windows/info.cpp
// =========================

#if !IS_OS_WINDOWS
	#error "This code is for Windows only!"
#endif

namespace os
{

// Name of OS without version
std::string name() { return "Windows"; }

// Name of OS + version
std::string pretty_name() { return info().pretty_name; }

// Codename of OS (if present)
std::string codename() { return ""; }

// Major, minor and patch of OS
::version version() { return info().version; }

// Version [+ some additional data]
std::string version_string() { return info().version_string; }

// Get whole OS info
const info_t& info()
{
    static info_t i;

    // Accessing WMI is expensive.
    // Init info only once.
    if (static bool init = true; init)
    {
        i.type = type();
        i.name = name();
        i.pretty_name = "Windows"; // Will be updated
        i.codename = codename();

        // KUSER_SHARED_DATA address.
        // Offsets are taken from http://terminus.rewolf.pl/terminus/structures/ntdll/_KUSER_SHARED_DATA_x64.html
        constexpr uintptr_t data_adress = uintptr_t{ 0x7ffe0000 };
        const uint32_t major = *reinterpret_cast<const uint32_t*>(data_adress + 0x26c);
        const uint32_t minor = *reinterpret_cast<const uint32_t*>(data_adress + 0x270);
        const uint32_t patch = *reinterpret_cast<const uint32_t*>(data_adress + 0x260);

        i.version = ::version{ major, minor, patch };
        i.version_string = i.version.str();

        i.pretty_name += " ";
        if (i.version >= ::version{ 10, 0 })
        {
            i.pretty_name += std::to_string(i.version.major);
        }
        else if (i.version >= ::version{ 6, 3 })
        {
            i.pretty_name += "8.1";
        }
        else if (i.version >= ::version{ 6, 2 })
        {
            i.pretty_name += "8";
        }
        else if (i.version >= ::version{ 6, 1 })
        {
            i.pretty_name += "7";
        }
        else if (i.version >= ::version{ 6, 0 })
        {
            i.pretty_name += "Vista";
        }
        else if (i.version >= ::version{ 5, 2 })
        {
            i.pretty_name += "XP 64-Bit Edition";
        }
        else if (i.version >= ::version{ 5, 1 })
        {
            i.pretty_name += "XP";
        }
        else if (i.version >= ::version{ 5, 0 })
        {
            i.pretty_name += "2000";
        }
        else
        {
            i.pretty_name.pop_back(); // remove space
        }

        init = false;
    }

    return i;
}

} // namespace os
// End of src/windows/info.cpp
// =========================

#endif // IS_OS_WINDOWS
