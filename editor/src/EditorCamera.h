#pragma once

#include <lotus/scene/Scene.h>

namespace Editor {
void SetupEditorCamera(const Lotus::SceneLoadEvent &event);
void MoveCamera(double deltaTime);
void RotateCamera(float xOffset, float yOffset);
} // namespace Editor
