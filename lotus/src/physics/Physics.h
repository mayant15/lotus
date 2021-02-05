#pragma once

#include "PxImpl.h"
#include "PhysicsInfo.h"

#include <lotus/physics/components.h>
#include <lotus/ecs/Event.h>
#include <lotus/ecs/Entity.h>

namespace Lotus::Physics
{
    /*** @brief Data for the physics system */
    struct State
    {
        ErrorCallback errorCallback {};
        SimulationEventCallback collisionCallbacks {};

        physx::PxDefaultAllocator allocator {};
        physx::PxFoundation* pFoundation = nullptr;
        physx::PxPhysics* pPhysics = nullptr;
        physx::PxDefaultCpuDispatcher* pDispatcher = nullptr;
        physx::PxPvd* pPVD = nullptr;
        physx::PxScene* pActiveScene = nullptr;

        // TODO: When the editor presses the play button, flip this to true
        bool isActive = false;
    };

    /** @brief Initialize physics systems and setup with project config */
    void OnInit(const InitEvent& event);

    /** @brief Create a physics scene and setup simulation */
    void OnBegin(const BeginEvent& event);

    /** @brief Physics simulation tick and result collection */
    void OnUpdate(const UpdateEvent& event);

    /** @brief Sync changes back to entities after update */
    void OnPostUpdate(const PostUpdateEvent& event);

    /** @brief Cleanup the system and close debug PVD connections */
    void OnDestroy(const DestroyEvent& event);

    /** @brief Register a rigidbody with the physics scene when a CRigidBody is added to an entity */
    void OnRigidBodyCreate(const ComponentCreateEvent<CRigidBody>& event);
    void OnRigidBodyDestroy(const ComponentDestroyEvent<CRigidBody>& event);
}
