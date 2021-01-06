#pragma once

#include "lotus/lotus_export.h"

template<int X>
static constexpr int BIT = (1 << X);

// TODO: Have sequential macros instead of this
constexpr int L_KEY_NONE = 0;
constexpr int L_KEY_ESC = 1;
constexpr int L_KEY_W   = 2;
constexpr int L_KEY_S   = 3;
constexpr int L_KEY_A   = 4;
constexpr int L_KEY_D   = 5;
constexpr int L_KEY_E   = 6;
constexpr int L_KEY_Q   = 7;
constexpr int L_KEY_R   = 8;

constexpr int L_KEY_PRESS   = 0;
constexpr int L_KEY_REPEAT  = 1;
constexpr int L_KEY_RELEASE = 2;

namespace Lotus
{
    enum class EEventType
    {
        NONE = 0,
        WINDOW_CLOSE_EVENT,
        KEYBOARD_EVENT,
        MOUSE_EVENT
    };

    struct Event
    {
        EEventType Type = EEventType::NONE;
        bool Immediate = false;
    };

    struct WindowCloseEvent : public Event
    {
        WindowCloseEvent()
        {
            Type = EEventType::WINDOW_CLOSE_EVENT;
            Immediate = true;
        }
    };

    struct WindowResizeEvent
    {
        float NewWidth;
        float NewHeight;
    };

    /**
     * Input events
     */

    struct KeyboardEvent : public Event
    {
        int KeyCode = L_KEY_NONE;
        int State = L_KEY_NONE;
        KeyboardEvent()
        {
            Type = EEventType::KEYBOARD_EVENT;
        }
    };

    struct MouseEvent : public Event
    {
        float MouseX = 0.0f;
        float MouseY = 0.0f;

        MouseEvent()
        {
            Type = EEventType::MOUSE_EVENT;
        }
    };
}
