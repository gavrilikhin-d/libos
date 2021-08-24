// Functions to get OS info. Header-only.

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
 *  // Functions to get OS info. Header-only.
 */

#pragma once

#include <string>

// #include "os/macros.h"
// ======================
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
// ======================


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


#if IS_OS_LINUX
// TODO: remove this dependency
// Needed for linux/info.cpp
// #include "os/kernel.hpp"
// ========================
namespace os::kernel
{

/**
 * @brief Get OS Kernel name
 *
 * @returns
 *  - Linux: `"Linux"`
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
// ========================


// linux/kernel.cpp
// ================
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
// ================


// linux/info.cpp
// ==============
#include <fstream>

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
            i.name = os::kernel::name();
            i.version = os::kernel::version();
            i.version_string = os::kernel::version_string();

            i.codename = "";
            i.pretty_name = i.name + " " + i.version_string;
        }

        init = false;
    }

    return i;
}

} // namespace os
// ==============
#endif // IS_OS_LINUX


#if IS_OS_WINDOWS
/* WMI to get OS info */
#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

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

        // Accessing WMI like here:
        // https://docs.microsoft.com/en-us/windows/win32/wmisdk/example--getting-wmi-data-from-the-local-computer

        HRESULT hres;

        // Step 1: --------------------------------------------------
        // Initialize COM. ------------------------------------------

        hres = CoInitializeEx(0, COINIT_MULTITHREADED);
        if (FAILED(hres)) { return i; }

        // Step 2: --------------------------------------------------
        // Set general COM security levels --------------------------

        hres = CoInitializeSecurity(
            NULL,
            -1,                          // COM authentication
            NULL,                        // Authentication services
            NULL,                        // Reserved
            RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
            RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
            NULL,                        // Authentication info
            EOAC_NONE,                   // Additional capabilities
            NULL                         // Reserved
        );


        if (FAILED(hres))
        {
            CoUninitialize();
            return i;
        }

        // Step 3: ---------------------------------------------------
        // Obtain the initial locator to WMI -------------------------

        IWbemLocator* pLoc = NULL;

        hres = CoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator, (LPVOID*)&pLoc);

        if (FAILED(hres))
        {
            CoUninitialize();
            return i;
        }

        // Step 4: -----------------------------------------------------
        // Connect to WMI through the IWbemLocator::ConnectServer method

        IWbemServices* pSvc = NULL;

        // Connect to the root\cimv2 namespace with
        // the current user and obtain pointer pSvc
        // to make IWbemServices calls.
        hres = pLoc->ConnectServer(
            _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
            NULL,                    // User name. NULL = current user
            NULL,                    // User password. NULL = current
            NULL,                    // Locale. NULL indicates current
            NULL,                    // Security flags.
            0,                       // Authority (for example, Kerberos)
            0,                       // Context object
            &pSvc                    // pointer to IWbemServices proxy
        );

        if (FAILED(hres))
        {
            pLoc->Release();
            CoUninitialize();
            return i;
        }


        // Step 5: --------------------------------------------------
        // Set security levels on the proxy -------------------------

        hres = CoSetProxyBlanket(
            pSvc,                        // Indicates the proxy to set
            RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
            RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
            NULL,                        // Server principal name
            RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
            RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
            NULL,                        // client identity
            EOAC_NONE                    // proxy capabilities
        );

        if (FAILED(hres))
        {
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return i;
        }

        // Step 6: --------------------------------------------------
        // Use the IWbemServices pointer to make requests of WMI ----

        // For example, get the name of the operating system
        IEnumWbemClassObject* pEnumerator = NULL;
        hres = pSvc->ExecQuery(
            bstr_t("WQL"),
            bstr_t("SELECT * FROM Win32_OperatingSystem"),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator);

        if (FAILED(hres))
        {
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return i;
        }

        // Step 7: -------------------------------------------------
        // Get the data from the query in step 6 -------------------

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;

        while (pEnumerator)
        {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if (uReturn == 0) { break; }

            VARIANT vtProp{};

            // Get the value of the Version property
            hr = pclsObj->Get(L"Version", 0, &vtProp, 0, 0);

            std::wstring v_wstr = vtProp.bstrVal;

            // Digits and points are ANSI symbols, so narrowing conversion from UTF-16 to UTF-8 is fine.
            std::string v_str(v_wstr.begin(), v_wstr.end());

            i.version = ::version{ v_str };
            i.version_string = v_str;

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

            VariantClear(&vtProp);
            pclsObj->Release();
        }

        // Cleanup
        // ========

        pSvc->Release();
        pLoc->Release();
        pEnumerator->Release();
        CoUninitialize();

        init = false;
    }

    return i;
}

} // namespace os
#endif // IS_OS_WINDOWS