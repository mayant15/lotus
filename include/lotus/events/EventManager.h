#pragma once

#include <lotus/Engine.h>
#include "Event.h"
#include "lotus/lcommon.h"
#include "lotus/internal/entt.hpp"

namespace Lotus
{
    /**
     * Wrapper class over EnTT to bind, queue and dispatch events.
     */
    class EventManager : public Singleton<EventManager>
    {
        entt::dispatcher _dispatcher{};

    public:

        /**
         * Invoke an event immediately
         * @tparam T Type of the event to invoke
         * @param event Data to invoke the event with
         */
        template<typename T>
        void Invoke(T& event)
        {
            _dispatcher.trigger<T>(event);
        }

        /**
         * Add an event to the queue, to be called at the end of the frame
         * @tparam T Type of the event to queue
         * @param event Data to queue the event with
         */
        template<typename T>
        void Queue(T event)
        {
            _dispatcher.enqueue<T>(event);
        }

        /**
         * Register an event callback which is a non-static member function
         * @tparam E Type of the event to register this callback for
         * @tparam F Pointer to the callback
         * @tparam L Type of the listener class
         * @param arg Listener class instance on which the callback is to be invoked
         */
        template<typename E, auto F, typename L>
        void Bind(L arg)
        {
            _dispatcher.sink<E>().template connect<F>(std::forward<L>(arg));
        }

        /**
         * Register a static or free function callback
         * @tparam E Type of the event to register this callback for
         * @tparam F Pointer to the callback
         */
        template<typename E, auto F>
        void Bind()
        {
            // Register a callback for an event
            _dispatcher.sink<E>().template connect<F>();
        }

        /**
         * Dispatch all queued events
         */
        void BroadcastQueue()
        {
            // Call all events in queue
            _dispatcher.update();
        }

    private:
        friend Singleton<EventManager>;

        EventManager() = default;
    };
}
