#pragma once


#include <lotus/lcommon.h>
#include <lotus/ecs/Entity.h>

#include "SceneTreeNode.h"

namespace Lotus
{
    // TODO: Make this just the scene hierarchy
    struct Scene
    {
        std::string Path;
        SceneTreeNode* Root = nullptr;
    };
}
