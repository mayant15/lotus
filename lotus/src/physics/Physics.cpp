#include "Physics.h"

#include <lotus/ecs/components/CTransform.h>
#include <lotus/Config.h>
#include <lotus/debug.h>

#define PX_RELEASE(x) if(x) { (x)->release(); (x) = nullptr; }
#define PVD_HOST "127.0.0.1"

namespace Lotus::Physics
{

    static Physics::State state {};

    PxRigidActor* createRigidBody(const PhysicsObjectInfo& info, const PxGeometry& geometry)
    {
        // TODO: Create physics materials
        const PxMaterial* mat = state.pPhysics->createMaterial(info.Material.StaticFriction,
                                                               info.Material.DynamicFriction,
                                                               info.Material.Restitution);
        PxShape* shape = state.pPhysics->createShape(geometry, *mat, true);

        // Position has already been set before this call
        const PxTransform transform(info.Collider->Position.x, info.Collider->Position.y, info.Collider->Position.z);
        PxRigidActor* actor;
        if (info.IsKinematic)
        {
            actor = PxCreateStatic(*state.pPhysics, transform, *shape);
        }
        else
        {
            actor = PxCreateDynamic(*state.pPhysics, transform, *shape, info.Material.Density);
        }

        // TODO: Enable/Disable can be handled here, but for a multiplier, I'll have to implement gravity myself
        if (info.Gravity < 0.2f)
        {
            actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
        }

        return actor;
    }

    void OnRigidBodyDestroy(const ComponentDestroyEvent<CRigidBody>& event)
    {
        // Sync physics changes with the transform component in the scene
        PxU32 nbActiveActors;
        PxActor** activeActors(state.pActiveScene->getActiveActors(nbActiveActors));
        for (PxU32 i = 0; i < nbActiveActors; i++)
        {
            auto actor = (PxRigidActor*) activeActors[i];
            auto id = (EntityID) (long long) (actor->userData);

            // Find the entity and remove it from the physics simulation
            if (event.entityID == id)
            {
                state.pActiveScene->removeActor(*actor);
                break;
            }
        }
    }

    void OnRigidBodyCreate(const ComponentCreateEvent<CRigidBody>& event)
    {
        auto* registry = GetRegistry();
        auto&&[rb, transform] = registry->get<CRigidBody, CTransform>(event.entityID);

        PhysicsObjectInfo info;
        info.Gravity = rb.Gravity;
        info.IsKinematic = rb.IsKinematic;

        // Material: Defaults are fine for now
        // TODO: Create a physics material asset and pass as an optional CRigidBody field

        // Setup the collider
        PhysicsColliderInfo colliderInfo;
        PxRigidActor* actor;

        info.Collider = &colliderInfo;
        if (registry->has<CSphereCollider>(event.entityID))
        {
            auto collider = registry->get<CSphereCollider>(event.entityID);
            colliderInfo.Position = collider.Position + transform.Position;

            const PxSphereGeometry geom(collider.Radius);
            actor = createRigidBody(info, geom);

            // NOTE: Entity IDs are just uint16_t's, so I cast that as a void* and store it with the object
            actor->userData = (void*) event.entityID;
            state.pActiveScene->addActor(*actor);
        }
        else if (registry->has<CBoxCollider>(event.entityID))
        {
            auto collider = registry->get<CBoxCollider>(event.entityID);
            colliderInfo.Position = collider.Position + transform.Position;

            const PxBoxGeometry geom(collider.Dimensions.x, collider.Dimensions.y, collider.Dimensions.z);
            actor = createRigidBody(info, geom);
            actor->userData = (void*) event.entityID;
            state.pActiveScene->addActor(*actor);
        }
        else if (registry->has<CCapsuleCollider>(event.entityID))
        {
            auto collider = registry->get<CCapsuleCollider>(event.entityID);
            colliderInfo.Position = collider.Position + transform.Position;

            const PxCapsuleGeometry geom(collider.Radius, collider.Height);
            actor = createRigidBody(info, geom);
            actor->userData = (void*) event.entityID;
            state.pActiveScene->addActor(*actor);
        }
        else
        {
            LOG_ERROR("Failed to add rigidbody to scene");
            throw std::invalid_argument("Invalid physics collider component");
        }
    }

    PxFilterFlags SampleFilterShader(
            PxFilterObjectAttributes attributes0, PxFilterData filterData0,
            PxFilterObjectAttributes attributes1, PxFilterData filterData1,
            PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
    {
//        // let triggers through
//        if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
//        {
//            pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
//            return PxFilterFlag::eDEFAULT;
//        }
//        // generate contacts for all that were not filtered above
//        pairFlags = PxPairFlag::eCONTACT_DEFAULT;
//
//        // trigger the contact callback for pairs (A,B) where
//        // the filtermask of A contains the ID of B and vice versa.
//        if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
//            pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

        pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND;
        return PxFilterFlag::eDEFAULT;
    }

    void createScene(const PhysicsSceneInfo& info)
    {
        LOG_INFO("Creating physics scene");

        PxSceneDesc sceneDesc(state.pPhysics->getTolerancesScale());
        sceneDesc.gravity = PxVec3(info.Gravity.x, info.Gravity.y, info.Gravity.z);

        state.pDispatcher = PxDefaultCpuDispatcherCreate(2);
        sceneDesc.cpuDispatcher = state.pDispatcher;
        sceneDesc.filterShader = SampleFilterShader;
        sceneDesc.simulationEventCallback = &state.collisionCallbacks;
        sceneDesc.flags = PxSceneFlag::eENABLE_ACTIVE_ACTORS | PxSceneFlag::eEXCLUDE_KINEMATICS_FROM_ACTIVE_ACTORS;

        state.pActiveScene = state.pPhysics->createScene(sceneDesc);

        PxPvdSceneClient* pvdClient = state.pActiveScene->getScenePvdClient();
        if (state.pPVD && pvdClient)
        {
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }
    }

    void OnInit(const InitEvent& event)
    {
        LOG_INFO("Creating physics world");
        state.pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, state.allocator, state.errorCallback);

        auto& conf = GetBuildConfig();
        if (conf.IsDebug)
        {
            state.pPVD = PxCreatePvd(*state.pFoundation);
            PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
            state.pPVD->connect(*transport, PxPvdInstrumentationFlag::eALL);
        }

        state.pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *state.pFoundation, PxTolerancesScale(), true, state.pPVD);
    }

    void OnBegin(const BeginEvent& event)
    {
        // TODO: Get from the SceneManager
        PhysicsSceneInfo info {};
        createScene(info);
        state.isActive = true;
    }

    void OnUpdate(const UpdateEvent& event)
    {
        if (state.isActive)
        {
            state.pActiveScene->simulate(event.DeltaTime);
            state.pActiveScene->fetchResults(true);
        }
    }

    void OnPostUpdate(const PostUpdateEvent& event)
    {
        // Sync physics changes with the transform component in the scene
        PxU32 nbActiveActors;
        PxActor** activeActors(state.pActiveScene->getActiveActors(nbActiveActors));
        auto* registry = GetRegistry();

        for (PxU32 i = 0; i < nbActiveActors; i++)
        {
            auto actor = (PxRigidActor*) activeActors[i];
            auto id = (EntityID) (long long) (actor->userData);

            // TODO: Change CTransform to hold a 4x4 matrix
            auto& transform = registry->get<CTransform>(id);
            auto position = actor->getGlobalPose().p;
            transform.Position = Vector3f { position.x, position.y, position.z };
        }
    }

    void OnDestroy(const DestroyEvent& event)
    {
        LOG_INFO("Destroying Physics...");
        PX_RELEASE(state.pActiveScene)
        PX_RELEASE(state.pDispatcher)
        PX_RELEASE(state.pPhysics)
        if (state.pPVD)
        {
            PxPvdTransport* transport = state.pPVD->getTransport();
            state.pPVD->release();
            state.pPVD = nullptr;
            PX_RELEASE(transport)
        }
        PX_RELEASE(state.pFoundation)
    }
}
