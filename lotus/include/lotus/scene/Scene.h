#pragma once


#include <lotus/lcommon.h>
#include <lotus/ecs/Entity.h>

#include "SceneTreeNode.h"

namespace Lotus
{
    // TODO: Make this just the scene hierarchy
    struct LOTUS_API Scene
    {
        std::string Path;
        SceneTreeNode* Root = nullptr;

        Scene();

        entt::registry* GetRegistry();

//        Entity GetParent(Entity entity);

        Entity CreateEntity();
        Entity CreateEntity(const std::string& path);

    private:
        entt::registry _registry;
    };

    struct SceneLoadEvent
    {
        IMMEDIATE_EVENT();
        Scene* pScene;
    };
}
