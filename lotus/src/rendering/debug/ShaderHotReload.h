#pragma once

#include <lotus/ecs/Event.h>

namespace Lotus
{
    struct ShaderModifyEvent : public Event
    {
        //
    };

    void ShaderHotReloadInit();
}
