#pragma once

#include <functional>

#define BIT(x) (1 << x)

namespace Lotus
{
    enum class EEventType
    {
        NONE = 0,
        WINDOW_CLOSE_EVENT,
        KEYBOARD_EVENT
    };

    // TODO: Do this with plain macros too?
    enum class EEventCategory
    {
        NONE = 0,
        WINDOW = BIT(0),
        INPUT = BIT(1),
        KEYBOARD = BIT(2),
        MOUSE = BIT(3)
    };

    inline EEventCategory operator|(EEventCategory A, EEventCategory B)
    {
        return static_cast<EEventCategory>(static_cast<int>(A) | static_cast<int>(B));
    }

    inline EEventCategory operator&(EEventCategory A, EEventCategory B)
    {
        return static_cast<EEventCategory>(static_cast<int>(A) & static_cast<int>(B));
    }

    struct Event
    {
        bool IsHandled = false;
        EEventCategory Category = EEventCategory::NONE;
        EEventType Type = EEventType::NONE;

        static EEventType GetStaticType()
        {
            return EEventType::NONE;
        }
    };

    struct UpdateEvent : public Event
    {
        float DeltaTime = 0.0f;
    };

    struct WindowCloseEvent : public Event
    {
        WindowCloseEvent()
        {
            Category = EEventCategory::WINDOW;
            Type = EEventType::WINDOW_CLOSE_EVENT;
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

    enum class EMouseCode
    {
        LEFT_MOUSE_BUTTON,
        RIGHT_MOUSE_BUTTON,
        MIDDLE_MOUSE_BUTTON,
        SCROLL
    };

    struct KeyboardEvent : public Event
    {
        int KeyCode = L_KEY_NONE;
        int State = L_KEY_NONE;
        KeyboardEvent()
        {
            Type = EEventType::KEYBOARD_EVENT,
            // TODO: This OR needs a == to got with it
//            Category = EEventCategory::KEYBOARD | EEventCategory::INPUT;
            Category = EEventCategory::INPUT;
            IsHandled = false;
        }

        static EEventType GetStaticType()
        {
            return EEventType::KEYBOARD_EVENT;
        }
    };

    enum class EMouseButtonState
    {
        PRESS,
        RELEASE
    };

    struct MouseButtonEvent : public Event
    {
        EMouseCode MouseCode;
        EMouseButtonState State;
    };

    struct MouseScrollEvent : public Event
    {
    };

    struct MouseMoveEvent : public Event
    {
    };
}
