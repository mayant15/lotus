#pragma once

#include <functional>
#include "lotus/lotus_export.h"

#define BIT(x) (1 << x)

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
        WINDOW = BIT(0),
        INPUT = BIT(1),
        KEYBOARD = BIT(2),
        MOUSE = BIT(3)
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
    };

    struct LOTUS_API UpdateEvent : public Event
    {
        float DeltaTime = 0.0f;
    };

    struct LOTUS_API WindowCloseEvent : public Event
    {
        WindowCloseEvent()
        {
            Category = EEventCategory::WINDOW;
            Type = EEventType::WINDOW_CLOSE_EVENT;
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

    // TODO: Have sequential macros instead of this
#define L_KEY_NONE 0
#define L_KEY_ESC  1
#define L_KEY_W    2
#define L_KEY_S    3
#define L_KEY_A    4
#define L_KEY_D    5
#define L_KEY_E    6
#define L_KEY_Q    7

#define L_KEY_PRESS   0
#define L_KEY_REPEAT  1
#define L_KEY_RELEASE 2

    struct LOTUS_API KeyboardEvent : public Event
    {
        int KeyCode = L_KEY_NONE;
        int State = L_KEY_NONE;
        KeyboardEvent()
        {
            Type = EEventType::KEYBOARD_EVENT,
            // TODO: This OR needs a == to got with it
//            Category = EEventCategory::KEYBOARD | EEventCategory::INPUT;
            Category = EEventCategory::INPUT;
        }
    };

    struct LOTUS_API MouseEvent : public Event
    {
//        int MouseCode;
//        int State;
        float MouseX;
        float MouseY;

        MouseEvent()
        {
            Type = EEventType::MOUSE_EVENT;
            Category = EEventCategory::INPUT;
        }
    };
}
