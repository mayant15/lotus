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

    /**
     * Base class to define lifecycle hooks
     */
    class ILifecycle
    {
    public:
        virtual ~ILifecycle() = default;

        [[maybe_unused]] virtual void OnInit(const InitEvent& event)
        {}

        [[maybe_unused]] virtual void OnBegin(const BeginEvent& event)
        {}

        [[maybe_unused]] virtual void OnPreUpdate(const PreUpdateEvent& event)
        {}

        [[maybe_unused]] virtual void OnUpdate(const UpdateEvent& event)
        {}

        [[maybe_unused]] virtual void OnPostUpdate(const PostUpdateEvent& event)
        {}

        [[maybe_unused]] virtual void OnPreDestroy(const PreDestroyEvent& event)
        {}

        [[maybe_unused]] virtual void OnDestroy(const DestroyEvent& event)
        {}

        [[maybe_unused]] virtual void OnShutdown(const ShutdownEvent& event)
        {}
    };
}
