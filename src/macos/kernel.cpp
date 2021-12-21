#include "os/kernel.hpp"

#include "os/macros.h"
#if !IS_OS_MACOS
    #error "This code is for macOS only!"
#endif

#include <sys/utsname.h>

namespace os::kernel
{

// Get name of OS kernel
std::string name() { return "Darwin"; }

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
        i.name = name();
        i.version = ::version{utsname.release};
        i.version_string = utsname.release;

        init = false;
    }

    return i;
}

} // namespace os::kernel