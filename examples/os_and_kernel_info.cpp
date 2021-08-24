#include <iostream>

#include "os/info.hpp"
#include "os/kernel.hpp"

int main()
{
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