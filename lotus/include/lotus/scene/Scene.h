#pragma once


#include "lotus/lcommon.h"
#include "lotus/ecs/ACamera.h"
#include "lotus/scene/SceneTreeNode.h"

namespace Lotus
{
    // TODO: Make this just the scene hierarchy
    struct Scene
    {
        std::string Path;
        SceneTreeNode* Root = nullptr;
        EntityID ActiveCamera;
    };
}
