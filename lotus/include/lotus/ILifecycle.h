#pragma once

#include "lotus/ecs/Event.h"

namespace Lotus
{

    struct InitEvent : Event
    {
        InitEvent()
        {
            Immediate = true;
        }
    };

    struct BeginEvent : Event
    {
        BeginEvent()
        {
            Immediate = true;
        }
    };

    struct PreUpdateEvent : Event
    {
        PreUpdateEvent()
        {
            Immediate = true;
        }
    };

    struct UpdateEvent : Event
    {
        double DeltaTime = 0.0f;

        UpdateEvent()
        {
            Immediate = true;
        }
    };

    struct PostUpdateEvent : Event
    {
        PostUpdateEvent()
        {
            Immediate = true;
        }
    };

    struct PreDestroyEvent : Event
    {
        PreDestroyEvent()
        {
            Immediate = true;
        }
    };

    struct DestroyEvent : Event
    {
    };

    struct ShutdownEvent : Event
    {
    };
}
