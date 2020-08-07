#pragma once

#define BIT(x) (1 << x)

namespace Lotus
{
    enum class EEventType
    {
        NONE = 0,
        WINDOW_CLOSE_EVENT
    };

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

    struct Event
    {
        bool IsHandled = false;
        EEventCategory Category = EEventCategory::NONE;
        EEventType Type = EEventType::NONE;
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

    enum class EKeyCode
    {
        ESC
    };

    enum class EKeyState
    {
        PRESS,
        REPEAT,
        RELEASE
    };

    enum class EMouseCode
    {
        LEFT_MOUSE_BUTTON,
        RIGHT_MOUSE_BUTTON,
        MIDDLE_MOUSE_BUTTON,
        SCROLL
    };

    struct KeyboardEvent : public Event
    {
        EKeyCode KeyCode;
        EKeyState State;
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
