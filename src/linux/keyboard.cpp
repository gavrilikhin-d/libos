#include "os/keyboard.hpp"

#include "os/macros.h"
#if !IS_OS_LINUX
    #error "This code is for Linux only!"
#endif

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#include "xlib_common.hpp"

namespace os::detail
{

/// DRY function for sending keys with XTest
void xtest_fake_key_events(const os::keyboard::combination &combo, bool is_press)
{
    Display *display = os::detail::display_handler::get().native();

    for (const auto &key : combo.keys)
    {
        XTestFakeKeyEvent(
            display,
            XKeysymToKeycode(
                display,
                static_cast<KeySym>(key) // Our vk values same as KeySym for linux
            ),
            is_press,
            0 // delay
        );
    }
}

} // namespace os::detail

namespace os::keyboard
{

// Check if every key in combination is pressed
bool is_pressed(const combination &combo)
{
    Display *display = os::detail::display_handler::get().native();

    char keys_return[32];
    XQueryKeymap(display, keys_return);

    for (const auto &key : combo.keys)
    {
        KeyCode kc = XKeysymToKeycode(display, static_cast<KeySym>(key));
        // Key not pressed
        if (!(keys_return[kc / 8] & (1 << (kc % 8)))) { return false; }
    }

    return true;
}

// Get combination of all pressed keys on a keyboard
combination pressed_keys()
{
    combination combo;

    Display *display = os::detail::display_handler::get().native();

    char keys_return[32];
    XQueryKeymap(display, keys_return);

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
    os::detail::xtest_fake_key_events(combo, true /*means press*/);
}

// Release combination of keys
void release(const combination &combo)
{
    os::detail::xtest_fake_key_events(combo, false /*means release*/);
}

} // namespace os::keyboard