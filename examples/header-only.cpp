#include <iostream>

#include "os/header-only/os.hpp"

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


    std::cout << "\n";

    // Shorten virtual keys name.
    // os::keyboard::vk::Shift -> vk::Shift
    using os::keyboard::vk;

    // ... or event better:
    // os::keyboard::vk::Shift -> Shift;
    // using enum os::keyboard::vk; // uncomment this line in C++20

    os::keyboard::release(vk::Enter); // Protect from instant skipping
    std::cout << "Please, press Enter to start." << std::flush; // Flush to see the message
    while (!os::keyboard::is_pressed(vk::Enter)) {} // vk::Control is the same

    std::cout << "\n";


    // Sends 'F' to... console? space? I don't know!
    // Just move your mouse input focus.
    //
    // In the future you will be able to change focus windows,
    // but now deal with it.
    //
    // May not produce 'F' if:
    //  - Keyboard layout is not english
    //  - Caps Lock is toggled
    //  - Keys has unusual mapping
    // Note: this is not a bug, it's a feature :)
    std::cout << "Here's F for you:" << std::endl;
    os::keyboard::click(vk::Shift + vk::F);

    // Click Enter if you want to... you know... enter it
    // /* Same as click(vk::Enter) */
    // os::keyboard::press(vk::Enter);
    // os::keyboard::release(vk::Enter);

    return 0;
}