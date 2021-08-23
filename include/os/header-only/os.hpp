// Header-only library

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

/** @file os/header-only/os.hpp
 *  Header-only library
 */

#pragma once

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


// #include "os/info.hpp"
// ======================
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
// ======================


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


// #include "os/keyboard.hpp"
// ==========================
#include <vector>

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
#elif IS_OS_WINDOWS
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
// ==========================


#if IS_OS_LINUX
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


// linux/keyboard.cpp
// ==================
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
// ==================
#endif // IS_OS_LINUX



#if IS_OS_WINDOWS
// windows/info.cpp
// ================
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
// ================


// windows/kernel.cpp
// ==================
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
        /* Versioning of the Windows kernel is the same as OS */
        i.version = os::version();
        i.version_string = os::version_string();

        init = false;
    }

    return i;
}

} // namespace os::kernel
// ==================


// windows/keyboard.cpp
// ====================
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
// ====================
#endif // IS_OS_WINDOWS