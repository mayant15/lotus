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

constexpr int L_KEY_PRESS   = 0;
constexpr int L_KEY_REPEAT  = 1;
constexpr int L_KEY_RELEASE = 2;

namespace Lotus
{
    enum class LOTUS_API EEventType
    {
        NONE = 0,
        WINDOW_CLOSE_EVENT,
        KEYBOARD_EVENT,
        MOUSE_EVENT
    };

    // TODO: Do this with plain macros too?
    enum class LOTUS_API EEventCategory
    {
        NONE = 0,
        WINDOW = BIT<0>,
        INPUT = BIT<1>,
        KEYBOARD = BIT<2>,
        MOUSE = BIT<3>
    };

    inline LOTUS_API EEventCategory operator|(EEventCategory A, EEventCategory B)
    {
        return static_cast<EEventCategory>(static_cast<int>(A) | static_cast<int>(B));
    }

    inline LOTUS_API EEventCategory operator&(EEventCategory A, EEventCategory B)
    {
        return static_cast<EEventCategory>(static_cast<int>(A) & static_cast<int>(B));
    }

    struct LOTUS_API Event
    {
        EEventCategory Category = EEventCategory::NONE;
        EEventType Type = EEventType::NONE;
        bool Immediate = false;
    };

    struct LOTUS_API WindowCloseEvent : public Event
    {
        WindowCloseEvent()
        {
            Category = EEventCategory::WINDOW;
            Type = EEventType::WINDOW_CLOSE_EVENT;
            Immediate = true;
        }
    };

    struct LOTUS_API WindowResizeEvent
    {
        float NewWidth;
        float NewHeight;
    };

    /**
     * Input events
     */

    struct LOTUS_API KeyboardEvent : public Event
    {
        int KeyCode = L_KEY_NONE;
        int State = L_KEY_NONE;
        KeyboardEvent()
        {
            Type = EEventType::KEYBOARD_EVENT;
            // TODO: This OR needs a == to got with it
//            Category = EEventCategory::KEYBOARD | EEventCategory::INPUT;
            Category = EEventCategory::INPUT;
        }
    };

    struct LOTUS_API MouseEvent : public Event
    {
        float MouseX = 0.0f;
        float MouseY = 0.0f;

        MouseEvent()
        {
            Type = EEventType::MOUSE_EVENT;
            Category = EEventCategory::INPUT;
        }
    };
}
