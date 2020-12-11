#pragma once

#include "lotus/lcommon.h"
#include "lotus/ecs/Event.h"

template<typename T>
constexpr auto CREATE_OF_TYPE = std::remove_reference<T>();

namespace Lotus
{
    /**
     * Wrapper class over EnTT to bind, queue and dispatch events.
     */
    class LOTUS_API EventManager : public Singleton<EventManager>
    {
        entt::dispatcher _dispatcher{};

    public:

        /**
         * @brief Dispatch an event
         *
         * Use this function to dispatch an event. If the `Immediate` property is true,
         * the registered callback will be immediately invoked. Otherwise, the event will
         * be queued and dispatched at the end of the frame.
         *
         * @tparam T Type parameter for the event to be dispatched
         * @param event Event object to be dispatched
         */
        template<typename T>
        void Dispatch(T event)
        {
            static_assert(std::is_base_of_v<Event, T>);
            if (event.Immediate)
            {
                _dispatcher.trigger<T>(event);
            }
            else
            {
                _dispatcher.enqueue<T>(event);
            }
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
            static_assert(std::is_base_of_v<Event, E>);
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
            static_assert(std::is_base_of_v<Event, E>);
            _dispatcher.sink<E>().template connect<F>();
        }

        /**
         * Dispatch all queued events
         */
        void DispatchAll() const
        {
            // Call all events in queue
            _dispatcher.update();
        }

    private:
        friend Singleton<EventManager>;
        EventManager() = default;
    };
}
