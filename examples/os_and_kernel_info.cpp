#include <iostream>

#include "os/info.hpp"
#include "os/kernel.hpp"

// Get library version
#include "os/libos.hpp"

int main()
{
    // Print library information. Also possible:
    // std::cout << "LibOS version: " << libos::version.str() << "\n";
    // std::cout << "LibOS version: " << LIBOS_VERSION_STRING << "\n";
    std::cout << "LibOS version: " << libos::version_string << "\n";

    std::cout << "\n";

    // Get OS type. Also possible to use #ifdef OS_<TYPE> or #if IS_OS_<TYPE>
    std::cout << "OS type: ";
    if constexpr (os::type() == os::linux) { std::cout << "linux";     }
    else if (os::type() == os::windows)    { std::cout << "windows";   }
    else if (os::type() == os::undefined)  { std::cout << "undefined"; }
    std::cout << "\n";

    std::cout
        << "OS name: " << os::name() << "\n"
        << "OS pretty name: " << os::pretty_name() << "\n"
        << "OS codename: " << os::codename() << "\n"
        // Needs explicit str() call.
        // Result may be not the same as version_string().
        << "OS version: " << os::version().str() << "\n"
        << "OS version string: " << os::version_string() << "\n";

    // Contains all the info above.
    // Better don't copy, if don't need it
    const os::info_t &info = os::info();


    std::cout << "\n";

    std::cout
        << "Kernel name: " << os::kernel::name() << "\n"
        // Needs explicit str() call.
        // Result may be not the same as version_string().
        << "Kernel version: " << os::kernel::version().str() << "\n"
        << "Kernel version string: " << os::kernel::version_string() << "\n";

    return 0;
}