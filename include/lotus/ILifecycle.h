#pragma once

#include "events/Event.h"

namespace Lotus
{

    struct InitEvent : Event {};
    struct BeginEvent : Event {};
    struct PreUpdateEvent : Event {};
    struct UpdateEvent : Event { float DeltaTime = 0.0f; };
    struct PostUpdateEvent : Event {};
    struct PreDestroyEvent : Event {};
    struct DestroyEvent : Event {};
    struct ShutdownEvent : Event {};

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
