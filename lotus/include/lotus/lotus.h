/**
 * Primary header file. This contains all necessary includes to use the engine.
 */

#pragma once

#include <string>
#include <filesystem>

#include "lcommon.h"
#include "filesystem.h"
#include "utility.h"

#include "physics/Vector.h"
#include "physics/components.h"

#include "ecs/components/CTransform.h"
#include "ecs/components/CCamera.h"
#include "ecs/components/CLight.h"
#include "ecs/ComponentRegistry.h"
#include "ecs/Entity.h"
#include "ecs/Event.h"
#include "ecs/EventManager.h"
#include "ecs/ISystem.h"

#include "resources/AssetRegistry.h"
#include "resources/HDRI.h"
#include "resources/Material.h"
#include "resources/Model.h"
#include "resources/Shader.h"
#include "resources/Texture.h"

#include "rendering/CMeshRenderer.h"
#include "rendering/RenderPass.h"

#include "scene/Scene.h"
#include "scene/SceneTreeNode.h"
#include "scene/SceneManager.h"

#include "Config.h"
#include "Engine.h"
#include "Input.h"
