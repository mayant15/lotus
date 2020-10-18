#include "PhysicsSubsystem.h"

#include "lotus/debug.h"

#include "core/SystemRegistry.h"

#define PX_RELEASE(x) if(x) { x->release(); x = nullptr; }
#define PVD_HOST "127.0.0.1"

namespace Lotus::Physics
{
    // TODO: Do not use entt types here
//    void onCRigidBodyConstruct(entt::registry& registry, entt::entity entity)
//    {
//        auto&& [rb, collider, transform] = registry.get<CRigidBody, CCollider, CTransform>(entity);
//
//        PhysicsObjectInfo info;
//        info.Gravity = rb.Gravity;
//        info.IsKinematic = rb.IsKinematic;
//
//        // TODO: Init collider info too
//
//        // TODO: Fire an event that the physics subsystem will catch
//        GetSubsystem<PhysicsSubsystem>().CreateRigidBody(info);
//    }

    PhysicsSubsystem::PhysicsSubsystem(bool usePVD)
    {
        LOG_INFO("Creating physics world");
        _pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

        if (usePVD)
        {
            _pPVD = PxCreatePvd(*_pFoundation);
            PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
            _pPVD->connect(*transport, PxPvdInstrumentationFlag::eALL);
        }

        _pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *_pFoundation, PxTolerancesScale(), true, _pPVD);

//        GET(ECS::EntityRegistry).BindOnConstruct<CRigidBody>(onCRigidBodyConstruct);
    }

    void PhysicsSubsystem::CreateScene(const PhysicsSceneInfo& info)
    {
        LOG_INFO("Creating physics scene");


        PxSceneDesc sceneDesc(_pPhysics->getTolerancesScale());
        sceneDesc.gravity = PxVec3(info.Gravity.x, info.Gravity.y, info.Gravity.z);

        _pDispatcher = PxDefaultCpuDispatcherCreate(2);
        sceneDesc.cpuDispatcher = _pDispatcher;
        sceneDesc.filterShader = PxDefaultSimulationFilterShader;

        _pActiveScene = _pPhysics->createScene(sceneDesc);

        PxPvdSceneClient* pvdClient = _pActiveScene->getScenePvdClient();
        if (_pPVD && pvdClient)
        {
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }
    }

    void PhysicsSubsystem::CreateRigidBody(const PhysicsObjectInfo& info) const
    {
        // TODO: Create physics materials
        const PxMaterial* mat = _pPhysics->createMaterial(info.Material.StaticFriction, info.Material.DynamicFriction, info.Material.Restitution);

        if (info.Collider->Shape == EPhysicsShape::SPHERE)
        {
            auto* collider = static_cast<PhysicsSphereColliderInfo*>(info.Collider);

            const PxSphereGeometry geom(collider->Radius);
            const PxTransform transform(collider->Position.x, collider->Position.y, collider->Position.z);

            PxShape* shape = _pPhysics->createShape(geom, *mat, true);

            PxRigidActor* actor;
            if (info.IsKinematic)
            {
                actor = PxCreateStatic(*_pPhysics, transform, *shape);
            }
            else
            {
                actor = PxCreateDynamic(*_pPhysics, transform, *shape, info.Material.Density);
            }
            _pActiveScene->addActor(*actor);
        }
    }


    void PhysicsSubsystem::Update(float delta) const
    {
        _pActiveScene->simulate(delta);
        _pActiveScene->fetchResults(true);
    }

    void PhysicsSubsystem::Shutdown()
    {
        LOG_INFO("Destroying Physics...");
        PX_RELEASE(_pActiveScene)
            PX_RELEASE(_pDispatcher)
            PX_RELEASE(_pPhysics)
            if (_pPVD)
            {
                PxPvdTransport* transport = _pPVD->getTransport();
                _pPVD->release();
                _pPVD = nullptr;
                PX_RELEASE(transport)
            }
        PX_RELEASE(_pFoundation)
    }
}
