/**
 * Primary header file. This contains all necessary includes to use the engine.
 */

#pragma once

#include <string>
#include <filesystem>

#include "lcommon.h"
#include "physics/physics.h"

#include "ecs/EntityRegistry.h"
#include "ecs/Entity.h"
#include "ecs/AActor.h"
#include "ecs/ACamera.h"
#include "ecs/ALight.h"
#include "ecs/Event.h"
#include "ecs/EventManager.h"
#include "ecs/ISystem.h"

#include "resources/AssetRegistry.h"
#include "resources/Cubemap.h"
#include "resources/Material.h"
#include "resources/Model.h"
#include "resources/Shader.h"
#include "resources/Texture.h"

#include "scene/SceneManager.h"

#include "Config.h"
#include "entry.h"
#include "ILifecycle.h"
#include "Input.h"
#include "python.h"
#include "Renderer.h"
