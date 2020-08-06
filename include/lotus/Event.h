#pragma once

#define BIT(x) (1 << x)

namespace Lotus
{
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
        bool IsHandled;
        EEventCategory Category;
    };

    struct WindowCloseEvent
    {
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
