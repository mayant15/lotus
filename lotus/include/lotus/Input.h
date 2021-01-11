#pragma once

#include "lotus/lcommon.h"
#include "lotus/ecs/Event.h"

namespace Lotus
{
    // TODO: Have sequential macros instead of this
    constexpr unsigned int L_KEY_NONE = 0;
    constexpr unsigned int L_KEY_ESC = 1;

    constexpr unsigned int L_KEY_A = 65;
    constexpr unsigned int L_KEY_B = 66;
    constexpr unsigned int L_KEY_C = 67;
    constexpr unsigned int L_KEY_D = 68;
    constexpr unsigned int L_KEY_E = 69;
    constexpr unsigned int L_KEY_F = 70;
    constexpr unsigned int L_KEY_G = 71;
    constexpr unsigned int L_KEY_H = 72;
    constexpr unsigned int L_KEY_I = 73;
    constexpr unsigned int L_KEY_J = 74;
    constexpr unsigned int L_KEY_K = 75;
    constexpr unsigned int L_KEY_L = 76;
    constexpr unsigned int L_KEY_M = 77;
    constexpr unsigned int L_KEY_N = 78;
    constexpr unsigned int L_KEY_O = 79;
    constexpr unsigned int L_KEY_P = 80;
    constexpr unsigned int L_KEY_Q = 81;
    constexpr unsigned int L_KEY_R = 82;
    constexpr unsigned int L_KEY_S = 83;
    constexpr unsigned int L_KEY_T = 84;
    constexpr unsigned int L_KEY_U = 85;
    constexpr unsigned int L_KEY_V = 86;
    constexpr unsigned int L_KEY_W = 87;
    constexpr unsigned int L_KEY_X = 88;
    constexpr unsigned int L_KEY_Y = 89;
    constexpr unsigned int L_KEY_Z = 90;

    constexpr unsigned int L_KEY_RELEASE = 0;
    constexpr unsigned int L_KEY_PRESS = 1;
    constexpr unsigned int L_KEY_REPEAT = 2;

    struct KeyboardEvent : public Event
    {
        unsigned int KeyCode = L_KEY_NONE;
        unsigned int State = L_KEY_NONE;

        KeyboardEvent()
        {
            Type = EEventType::KEYBOARD_EVENT;
        }
    };

    struct MouseEvent : public Event
    {
        double MouseX = 0.0f;
        double MouseY = 0.0f;

        MouseEvent()
        {
            Type = EEventType::MOUSE_EVENT;
        }
    };
}

/**
 * Input manager that receives window events and maintains input state. Users should query this class
 * to get input information. This is to avoid end users interacting with low level window implementations
 * themselves.
 */
namespace Lotus::Input
{
    /**
     * KeyboardEvent receiver that updates internal keyboard state.
     * @param event Keyboard input data
     */
    LOTUS_API void UpdateKeyState(const KeyboardEvent& event);

    /**
     * MouseEvent receiver that updates internal mouse state.
     * @param event Mouse input data
     */
    LOTUS_API void UpdateMouseState(const MouseEvent& event);

    /**
     * Query the keyboard state for a particular key
     * @param key Keycode for the requested key
     * @return True if the key is pressed
     */
    LOTUS_API bool GetKeyPressed(int key);

    /**
     * Query the mouse state for mouse cursor delta since the last mouse event
     * @return Pair of floats with X and Y deltas
     */
    LOTUS_API std::pair<float, float> GetMouseDelta();
}
