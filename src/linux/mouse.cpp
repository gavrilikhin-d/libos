#include "os/mouse.hpp"

#include "os/macros.h"
#if !IS_OS_LINUX
    #error "This code is for Linux only!"
#endif

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#include "xlib_common.hpp"

namespace os::detail
{

/// DRY function for sending button event with XTest
void xtest_fake_button_event(os::mouse::button b, bool is_press)
{
    Display *display = os::detail::display_handler::get().native();

    XTestFakeButtonEvent(
        display,
        static_cast<unsigned>(b), // Out buttons have the same values as XLib's
        is_press,
        0 // delay
    );
}

} // namespace os::detail

namespace os::mouse
{

/// Check if button is pressed
bool is_pressed(button b);

/// Press mouse button (untill release())
void press(button b)
{
    os::detail::xtest_fake_button_event(b, true /* means press */);
}
/// Release mouse button
void release(button b)
{
    os::detail::xtest_fake_button_event(b, false /* means release */);
}


/// Scroll wheel up or down
void scroll(scroll_direction d);
/// Check if wheel is scrolling at the moment
bool scrolling();

/// Get current cursor position
point position();
/// Move cursor to a new position
void move_to(point pos);


} // namespace os::mouse