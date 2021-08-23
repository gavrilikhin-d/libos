#include "os/kernel.hpp"

#include "os/macros.h"
#if !IS_OS_WINDOWS
    #error "This code is for Windows only!"
#endif

#include "os/info.hpp"

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