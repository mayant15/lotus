#include "Physics.h"

#include <lotus/ecs/components/CTransform.h>
#include <lotus/Config.h>
#include <lotus/debug.h>

#define PX_RELEASE(x) if(x) { (x)->release(); (x) = nullptr; }
#define PVD_HOST "127.0.0.1"

namespace Lotus::Physics
{
    static Physics::State state {};

    static Vector3f toVector3f(const physx::PxVec3& vec)
    {
        return { vec.x, vec.y, vec.z };
    }

    static physx::PxVec3 toPxVec3(const Vector3f& vec)
    {
        return { vec.x, vec.y, vec.z };
    }

    static PxQuat toPxQuat(const Vector3f& vec)
    {
        // Copy pasting this for now. Please work, will improve later.
        // https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Source_code
        // yaw (Z), pitch (Y), roll (X)
        // Abbreviations for the various angular functions
        const double yaw = glm::radians(vec.y);
        const double pitch = glm::radians(vec.x);
        const double roll = glm::radians(vec.z);

        const double cy = glm::cos(yaw * 0.5);
        const double sy = glm::sin(yaw * 0.5);
        const double cp = glm::cos(pitch * 0.5);
        const double sp = glm::sin(pitch * 0.5);
        const double cr = glm::cos(roll * 0.5);
        const double sr = glm::sin(roll * 0.5);

        PxQuat q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;

        return q;
    }

    PxRigidActor* createRigidBody(const PhysicsObjectInfo& info, const PxGeometry& geometry)
    {
        // TODO: Create physics materials
        const PxMaterial* mat = state.pPhysics->createMaterial(info.Material.StaticFriction,
                                                               info.Material.DynamicFriction,
                                                               info.Material.Restitution);
        PxShape* shape = state.pPhysics->createShape(geometry, *mat, true);

        // Position and rotation has already been set before this call
        const PxTransform transform {
            toPxVec3(info.Collider->Position),
            toPxQuat(info.Collider->Rotation)
        };
        if (info.IsKinematic)
        {
            return (PxRigidActor*) PxCreateStatic(*state.pPhysics, transform, *shape);
        }
        else
        {
            return (PxRigidActor*) PxCreateDynamic(*state.pPhysics, transform, *shape, info.Material.Density);
        }
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

    // TODO: This should also happen on rigidbody or collider *changes*
    void OnRigidBodyCreate(const ComponentCreateEvent<CRigidBody>& event)
    {
        auto* registry = state.pEngineScene->GetRegistry();
        auto&&[rb, transform] = registry->get<CRigidBody, CTransform>(event.entityID);

        PhysicsObjectInfo info;
        info.Gravity = rb.Gravity;
        info.IsKinematic = rb.IsKinematic;

        // Material: Defaults are fine for now
        // TODO: Create a physics material asset and pass as an optional CRigidBody field

        // Setup the collider
        PhysicsColliderInfo colliderInfo;

        info.Collider = &colliderInfo;
        if (registry->has<CSphereCollider>(event.entityID))
        {
            auto collider = registry->get<CSphereCollider>(event.entityID);
            colliderInfo.Position = collider.Position + transform.Position;
            colliderInfo.Rotation = transform.Rotation;

            const PxSphereGeometry geom(collider.Radius);
            rb.detail.actor = createRigidBody(info, geom);

            // TODO: Enable/Disable can be handled here, but for a multiplier, I'll have to implement gravity myself
            if (info.Gravity < 0.2f)
            {
                rb.detail.actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
            }

            // NOTE: Entity IDs are just uint16_t's, so I cast that as a void* and store it with the object
            rb.detail.actor->userData = (void*) event.entityID;
            state.pActiveScene->addActor(*rb.detail.actor);
        }
        else if (registry->has<CBoxCollider>(event.entityID))
        {
            auto collider = registry->get<CBoxCollider>(event.entityID);
            colliderInfo.Position = collider.Position + transform.Position;
            colliderInfo.Rotation = transform.Rotation;

            const PxBoxGeometry geom(collider.Dimensions.x, collider.Dimensions.y, collider.Dimensions.z);
            rb.detail.actor = createRigidBody(info, geom);

            // TODO: Enable/Disable can be handled here, but for a multiplier, I'll have to implement gravity myself
            if (info.Gravity < 0.2f)
            {
                rb.detail.actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
            }

            rb.detail.actor->userData = (void*) event.entityID;
            state.pActiveScene->addActor(*rb.detail.actor);
        }
        else if (registry->has<CCapsuleCollider>(event.entityID))
        {
            auto collider = registry->get<CCapsuleCollider>(event.entityID);
            colliderInfo.Position = collider.Position + transform.Position;
            colliderInfo.Rotation = transform.Rotation;

            const PxCapsuleGeometry geom(collider.Radius, collider.Height);
            rb.detail.actor = createRigidBody(info, geom);

            // TODO: Enable/Disable can be handled here, but for a multiplier, I'll have to implement gravity myself
            if (info.Gravity < 0.2f)
            {
                rb.detail.actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
            }

            rb.detail.actor->userData = (void*) event.entityID;
            state.pActiveScene->addActor(*rb.detail.actor);
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

    void OnSceneLoad(const SceneLoadEvent& event)
    {
        // TODO: Get from the SceneManager
        PhysicsSceneInfo info {};
        createScene(info);
        state.pEngineScene = event.pScene;
    }

    void OnSimulationBegin(const SimulationBeginEvent& event)
    {
        state.isActive = true;
    }

    void OnSimulationPause(const SimulationPauseEvent& event)
    {
        state.isActive = false;
    }

    void OnSimulationEnd(const SimulationEndEvent& event)
    {
        state.isActive = false;
        // TODO: Restore state here or in pre-update?
    }

    void OnPreUpdate(const PreUpdateEvent& event)
    {
        if (!state.isActive) return;

        // TODO: Copy only the changed information
        // Sync physics changes with the transform component in the scene
        auto* registry = state.pEngineScene->GetRegistry();
        auto view = registry->view<CRigidBody>();
        for (const auto& e : view)
        {
            auto& rb = view.get(e);
            auto& tf = registry->get<CTransform>(e);

            // TODO: Set rotation and scale too
            // TODO: If statements for each collider shape, add those offsets to the transform
            PxTransform pt {};
            pt.p = toPxVec3(tf.Position);
            pt.q = rb.detail.actor->getGlobalPose().q;
            rb.detail.actor->setGlobalPose(pt);
        }
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
        if (!state.isActive) return;

        // Sync physics changes with the transform component in the scene
        PxU32 nbActiveActors;
        PxActor** activeActors(state.pActiveScene->getActiveActors(nbActiveActors));
        auto* registry = state.pEngineScene->GetRegistry();

        for (PxU32 i = 0; i < nbActiveActors; i++)
        {
            try
            {
                auto actor = (PxRigidActor*) activeActors[i];
                auto id = (EntityID) (long long) (actor->userData);

                // TODO: Change CTransform to hold a 4x4 matrix
                if (registry->valid(id))
                {
                    auto& transform = registry->get<CTransform>(id);
                    auto position = actor->getGlobalPose().p;
                    transform.Position = toVector3f(position);
                }
            }
            catch (const std::exception& e)
            {
                LOG_WARN("Failed to update physics state. {}", e.what());
            }
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

    void ApplyForce(const CRigidBody& rb, Vector3f force, EForceType type)
    {
        using namespace physx;
        auto* body = reinterpret_cast<PxRigidBody*>(rb.detail.actor);
        auto pxforce = toPxVec3(force);
        switch (type)
        {
            case EForceType::IMPULSE:
                body->addForce(pxforce, PxForceMode::eIMPULSE);
                break;
            case EForceType::FORCE:
                body->addForce(pxforce, PxForceMode::eFORCE);
                break;
        }
    }
}
