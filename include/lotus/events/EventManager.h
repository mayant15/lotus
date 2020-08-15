#pragma once

#include <lotus/Engine.h>
#include "Event.h"
#include "lotus/lcommon.h"
#include "lotus/internal/entt.hpp"

namespace Lotus
{
    class EventManager : public Singleton<EventManager>
    {
        entt::dispatcher _dispatcher{};

    private:
        friend Singleton<EventManager>;

        EventManager() = default;

    public:
        template<typename T>
        void Invoke(T& event)
        {
            // Calls the callback for the event immediately
            _dispatcher.trigger<T>(event);
        }

        // Add an event to the queue, to be called at the end of the frame
        template<typename T>
        void Queue(T event)
        {
            _dispatcher.enqueue<T>(event);
        }

        template<typename E, auto F, typename L>
        void Bind(L arg)
        {
            _dispatcher.sink<E>().template connect<F>(std::forward<L>(arg));
        }

        template<typename E, auto F>
        void Bind()
        {
            // Register a callback for an event
            _dispatcher.sink<E>().template connect<F>();
        }

        void BroadcastQueue()
        {
            // Call all events in queue
            _dispatcher.update();
        }
    };
}
