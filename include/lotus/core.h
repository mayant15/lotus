#pragma once

#include "internal/entt.hpp"

namespace Lotus
{
    void init();

    void run();

    void shutdown();

    struct IComponent
    {
    };

    typedef entt::registry EntityRegistry;
    typedef entt::entity EntityID;

    class IEntity
    {
    protected:
        EntityID ID{};
    public:
        virtual void start() = 0;
        virtual void update() = 0;
    };
}
