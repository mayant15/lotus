#pragma once

#include <lotus/ecs/Event.h>

namespace Editor
{
    // https://github.com/skypjack/entt/issues/604
    // https://github.com/skypjack/entt/blob/master/test/lib/dispatcher_plugin/plugin.cpp
    // https://github.com/skypjack/entt/blob/master/test/lib/dispatcher_plugin/main.cpp
    //  TODO: Need to specialize type_seq and make it point to the same context
    struct KeyboardEvent {};
    struct MouseEvent {};
    struct MouseButtonEvent {};
}
