#pragma once

#include <events.h>

namespace Editor
{
    void OnSceneLoad(const SceneLoadEvent& event);
    void RotateCamera(float xOffset, float yOffset);
}
