#include "os/info.hpp"

#include "os/macros.h"
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