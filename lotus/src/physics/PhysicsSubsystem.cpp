#include "PhysicsSubsystem.h"

#include "lotus/debug.h"
#include "lotus/ecs/components/CTransform.h"

#define PX_RELEASE(x) if(x) { (x)->release(); (x) = nullptr; }
#define PVD_HOST "127.0.0.1"

namespace Lotus::Physics
{
    void PhysicsSubsystem::OnRigidbodyCreate(const ComponentCreateEvent<CRigidBody>& event)
    {
        LOG_INFO("Creating a new rigidbody");
        auto* registry = GetRegistry();
        auto&&[rb, collider, transform] = registry->get<CRigidBody, CCollider, CTransform>(event.entityID);
        PhysicsObjectInfo info;

        // Rigid body
        info.Gravity = rb.Gravity;
        info.IsKinematic = rb.IsKinematic;

        // Material: Defaults are fine for now
        // TODO: Create a physics material asset and pass as an optional CRigidBody field

        // Collider
        // TODO: Handle other shapes
        PhysicsColliderInfo colliderInfo;
        colliderInfo.Position = collider.Position;
        info.Collider = &colliderInfo;

        PxShape* shape;
        if (collider.Shape == EPhysicsShape::SPHERE)
        {
            const PxSphereGeometry geom(collider.Radius);
            createRigidBody(info, geom);
        }
//        else if (collider.Shape == EPhysicsShape::CAPSULE)
//        {
//            const PxCapsuleGeometry geom(collider.Radius, collider.Height);
//            createRigidBody(info, geom);
//        }
//        else if (collider.Shape == EPhysicsShape::BOX)
//        {
//            const PxBoxGeometry geom(collider.Dimensions.x, collider.Dimensions.y, collider.Dimensions.z);
//            createRigidBody(info, geom);
//        }
        else
        {
            LOG_ERROR("Failed to add rigidbody to scene");
            throw std::invalid_argument ("Invalid physics collider component");
        }
    }

    void PhysicsSubsystem::createRigidBody(const PhysicsObjectInfo& info, const PxGeometry& geometry) const
    {
        // TODO: Create physics materials
        const PxMaterial* mat = _pPhysics->createMaterial(info.Material.StaticFriction, info.Material.DynamicFriction,
                                                          info.Material.Restitution);
        PxShape* shape = _pPhysics->createShape(geometry, *mat, true);
        const PxTransform transform(info.Collider->Position.x, info.Collider->Position.y, info.Collider->Position.z);
        PxRigidActor* actor;
        if (info.IsKinematic)
        {
            actor = PxCreateStatic(*_pPhysics, transform, *shape);
        } else
        {
            actor = PxCreateDynamic(*_pPhysics, transform, *shape, info.Material.Density);
        }
        _pActiveScene->addActor(*actor);
    }

    void PhysicsSubsystem::createScene(const PhysicsSceneInfo& info)
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

    void PhysicsSubsystem::OnInit(const InitEvent& event)
    {
        LOG_INFO("Creating physics world");
        _pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

        if (_usePVD)
        {
            _pPVD = PxCreatePvd(*_pFoundation);
            PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
            _pPVD->connect(*transport, PxPvdInstrumentationFlag::eALL);
        }

        _pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *_pFoundation, PxTolerancesScale(), true, _pPVD);


    }

    void PhysicsSubsystem::OnBegin(const BeginEvent& event)
    {
        PhysicsSceneInfo info {};
        createScene(info);
    }

    void PhysicsSubsystem::OnPreUpdate(const PreUpdateEvent& event)
    {
        // TODO: Fetch transform changes from the scene
    }

    void PhysicsSubsystem::OnUpdate(const UpdateEvent& event)
    {
        _pActiveScene->simulate(event.DeltaTime);
        _pActiveScene->fetchResults(true);
    }

    void PhysicsSubsystem::OnPostUpdate(const PostUpdateEvent& event)
    {
        // TODO: Push simulated transform changes to the scene
    }

    void PhysicsSubsystem::OnPreDestroy(const PreDestroyEvent& event)
    {
    }

    void PhysicsSubsystem::OnDestroy(const DestroyEvent& event)
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

    void PhysicsSubsystem::OnShutdown(const ShutdownEvent& event)
    {
    }
}
