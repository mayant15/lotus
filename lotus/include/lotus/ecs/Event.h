#pragma once

#include "lotus/lotus_export.h"

template<int X>
static constexpr int BIT = (1 << X);

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
}
