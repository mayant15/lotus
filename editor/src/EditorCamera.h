#pragma once

#include <events.h>

namespace Editor
{
    void OnSceneLoad(const SceneLoadEvent& event);
    void MoveCamera(double deltaTime);
    void RotateCamera(float xOffset, float yOffset);
}
