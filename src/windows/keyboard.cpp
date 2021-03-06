#include "os/keyboard.hpp"

#include "os/macros.h"
#if !IS_OS_WINDOWS
    #error "This code is for Windows only!"
#endif

#include <Windows.h>

namespace os::detail
{
    void send_inputs(const keyboard::combination &combo, bool is_down)
    {
        std::vector<INPUT> inputs(combo.keys.size());
        size_t i = 0;
        for (auto key : combo.keys)
        {
            auto& in = inputs[i];

            in.type = INPUT_KEYBOARD;
            in.ki.wVk = static_cast<int>(key);
            if (!is_down) 
            {
                in.ki.dwFlags = KEYEVENTF_KEYUP;
            }

            i++;
        }
        SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
    }
}

namespace os::keyboard
{

    // Check if every key in combination is pressed
    bool is_pressed(const combination& combo)
    {
        for (const auto& key : combo.keys)
        {
            short state = GetAsyncKeyState(static_cast<int>(key));
            // If the most significant bit of 2 bytes is not set, the key isn't pressed
            if (!(state & (1 << 15))) { return false; };
        }
        return true;
    }

    // Get combination of all pressed keys on a keyboard
    combination pressed_keys()
    {
        combination combo;

        // Go through every virtual key
        for (int key = 0; key < 256; ++key)
        {
            short state = GetAsyncKeyState(key);
            // If the most significant bit of 2 bytes set, the key is pressed
            if (state & (1 << 15)) { combo += static_cast<vk>(key); };
        }

        return combo;
    }

    // Press combination of keys (until release)
    void press(const combination& combo) { detail::send_inputs(combo, true); }

    // Release combination of keys
    void release(const combination& combo) { detail::send_inputs(combo, false); }

} // namespace os::keyboard