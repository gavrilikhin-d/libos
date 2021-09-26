// Semantic versioning. Header-only

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

/** @file os/header-only/version.hpp
 *  Semantic versioning. Header-only
 */

#pragma once

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