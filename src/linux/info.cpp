#include "os/info.hpp"

#include "os/macros.h"
#if !IS_OS_LINUX
    #error "This code is for Linux only!"
#endif

#include <fstream>

#include "os/kernel.hpp"

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