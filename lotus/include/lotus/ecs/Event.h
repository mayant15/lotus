#pragma once

#include "lotus/lotus_export.h"

/**
 * @brief Declare an immediate event
 *
 * There are two kinds of events, immediate or non-immediate. Immediate events are dispatched immediately,
 * non-immediate ones are handled at the end of frame. An event itself is just a struct, with whatever data you want
 * it to carry. To mark an event as immediate, add a public static boolean field \p immediate with the value true.
 * The IMMEDIATE_EVENT() macro function provides a shortcut to generate this statement. For example
 *
 * \example
 * \code
 * struct CustomEvent { int data = 0; };
 * struct CustomImmediateEvent
 * {
 *     IMMEDIATE_EVENT();
 *     int data = 0;
 * };
 */
#define IMMEDIATE_EVENT() \
    static constexpr bool immediate = true;

namespace Lotus
{
    /**
     * @brief Determine if an event is immediate
     * @tparam T Event to query for
     */
    template <typename T>
    struct is_immediate
    {
    private:
        using yes = char[1];
        using no = char[2];

        template <typename C> static yes& test_function(decltype(&C::immediate)) {};
        template <typename C> static no& test_function(...) {};

    public:
        /** @brief true if the event has a field \p immediate which is set to true */
        static constexpr bool value = []{
            if constexpr ( sizeof (test_function<T>(nullptr)) == sizeof (yes) )
                return T::immediate;
            return false;
        }();
    };

    struct KeyboardEvent {};
    struct MouseEvent {};
    struct MouseButtonEvent {};

    struct InitEvent { IMMEDIATE_EVENT() };
    struct BeginEvent { IMMEDIATE_EVENT() };
    struct PreUpdateEvent { IMMEDIATE_EVENT() };

    struct UpdateEvent
    {
        IMMEDIATE_EVENT();
        double DeltaTime = 0.0f;
    };

    struct PostUpdateEvent { IMMEDIATE_EVENT() };
    struct PreDestroyEvent { IMMEDIATE_EVENT() };
    struct DestroyEvent { IMMEDIATE_EVENT() };
    struct ShutdownEvent { IMMEDIATE_EVENT() };

    struct CollisionEvent {};
}
