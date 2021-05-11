#pragma once

#include <lotus/lcommon.h>
#include <lotus/ecs/Event.h>
#include "SceneTreeNode.h"
#include "Scene.h"

namespace Lotus::SceneManager
{
    LOTUS_API void LoadScene(const std::string& relpath);
    LOTUS_API void SaveScene();
    LOTUS_API SRef<Scene> GetCurrentScene();
    LOTUS_API void OnSimulationBegin(const SimulationBeginEvent& event);
    LOTUS_API void OnSimulationEnd(const SimulationEndEvent& event);
}
