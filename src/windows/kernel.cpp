#include "os/kernel.hpp"

#include "os/macros.h"
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