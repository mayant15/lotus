/**
 * Primary header file. This contains all necessary includes to use the engine.
 */

#pragma once

#include <string>
#include <filesystem>
#define RESOURCE(x) resource(__FILE__, x)

#include "physics.h"

#include "components/scene.h"
#include "components/lights.h"
#include "components/rendering.h"

#include "resources/AssetRegistry.h"
#include "resources/Cubemap.h"
#include "resources/Model.h"
#include "resources/Texture.h"

#include "events/Event.h"
#include "events/IWindow.h"
#include "events/Input.h"

#include "rendering/LShader.h"
#include "rendering/Renderer.h"

#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include "scene/AActor.h"
#include "scene/ACamera.h"

#include "python.h"
#include "Engine.h"

inline std::string resource(const std::string& root, const std::string& relPath)
{
    std::filesystem::path mainPath = root;
    return mainPath.parent_path().append("/resources/" + relPath).string();
}
