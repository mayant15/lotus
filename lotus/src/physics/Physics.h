#pragma once

#include "BulletImpl.h"
#include "PhysicsInfo.h"

#include <lotus/ecs/Entity.h>
#include <lotus/ecs/Event.h>
#include <lotus/physics/components.h>
#include <lotus/scene/Scene.h>

namespace Lotus::Physics {

/*** @brief Data for the physics system */
struct State
{
    World *pActiveScene;

    // TODO: When the editor presses the play button, flip this to true
    bool isActive = false;
    Scene *pEngineScene = nullptr;
    Observer preUpdateObserver{};
};

/** @brief Initialize physics systems and setup with project config */
void OnInit(const InitEvent &event);

/** @brief Create a physics scene and setup simulation */
void OnSceneLoad(const SceneLoadEvent &event);

/** @brief Start simulation when play pressed */
void OnSimulationBegin(const SimulationBeginEvent &event);

/** @brief Pause simulation when pause pressed */
void OnSimulationPause(const SimulationPauseEvent &event);

/** @brief Stop the simulation and restore physics state when stop pressed */
void OnSimulationEnd(const SimulationEndEvent &event);

/** @brief Sync changes from the entity registry to the physics world */
void OnPreUpdate(const PreUpdateEvent &event);

/** @brief Physics simulation tick and result collection */
void OnUpdate(const UpdateEvent &event);

/** @brief Sync changes back to entities after update */
void OnPostUpdate(const PostUpdateEvent &event);

/** @brief Cleanup the system and close debug PVD connections */
void OnDestroy(const DestroyEvent &event);

/** @brief Register a rigidbody with the physics scene when a CRigidBody is added to an entity */
void OnRigidBodyCreate(const ComponentCreateEvent<CRigidBody> &event);
void OnRigidBodyDestroy(const ComponentDestroyEvent<CRigidBody> &event);
} // namespace Lotus::Physics
