/**
 * This file defines the interface that the engine and the renderer will use to interact
 * with the physics subsystem.
 */


#pragma once

#include "PxImpl.h"
#include "lotus/physics.h"

#include <physx/PxPhysicsAPI.h>

namespace Lotus::Physics
{
    struct PhysicsSceneInfo
    {
        Vector3f Gravity = Vector3f(0.0f, -9.81f, 0.0f);
    };

    using namespace physx;
    class PhysicsSubsystem
    {

        PxDefaultAllocator _allocator;
        PhysXErrorCallback _errorCallback;
        PxFoundation* _pFoundation = nullptr;
        PxPhysics* _pPhysics = nullptr;
        PxDefaultCpuDispatcher* _pDispatcher = nullptr;
        PxPvd* _pPVD = nullptr;

        PxScene* _pActiveScene = nullptr;

    public:
        PhysicsSubsystem(bool usePVD = true);
        void CreateScene(const PhysicsSceneInfo& info);
        void Update(float delta) const;
        void Shutdown();
    };
}
