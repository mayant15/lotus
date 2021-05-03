#pragma once

#include <lotus/lcommon.h>
#include "SceneTreeNode.h"
#include "Scene.h"

namespace Lotus::SceneManager
{
    LOTUS_API void LoadScene(const std::string& path);
    LOTUS_API SRef<Scene> GetCurrentScene();
}
