#include "os/keyboard.hpp"

#include "os/macros.h"
#if !IS_OS_LINUX
    #error "This code is for Linux only!"
#endif

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