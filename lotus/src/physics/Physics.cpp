#include "Physics.h"

#include <lotus/Config.h>
#include <lotus/debug.h>
#include <lotus/ecs/components/CTransform.h>

namespace Lotus::Physics {

static Physics::State state{};

static btQuaternion toBtQuaternion(const Vector3f &vec)
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

    const double w = cr * cp * cy + sr * sp * sy;
    const double x = sr * cp * cy - cr * sp * sy;
    const double y = cr * sp * cy + sr * cp * sy;
    const double z = cr * cp * sy - sr * sp * cy;

    return {x, y, z, w};
}

// PxRigidActor* createRigidBody(const PhysicsObjectInfo& info, const PxGeometry& geometry)
// {
//     // TODO: Create physics materials
//     const PxMaterial* mat = state.pPhysics->createMaterial(info.Material.StaticFriction,
//                                                            info.Material.DynamicFriction,
//                                                            info.Material.Restitution);
//     PxShape* shape = state.pPhysics->createShape(geometry, *mat, true);

//     // Position and rotation has already been set before this call
//     const PxTransform transform {
//         toPxVec3(info.Collider->Position),
//         toPxQuat(info.Collider->Rotation)
//     };
//     if (info.IsKinematic)
//     {
//         return (PxRigidActor*) PxCreateStatic(*state.pPhysics, transform, *shape);
//     }
//     else
//     {
//         return (PxRigidActor*) PxCreateDynamic(*state.pPhysics, transform, *shape, info.Material.Density);
//     }
// }

void OnRigidBodyDestroy(const ComponentDestroyEvent<CRigidBody> &event)
{
    // Sync physics changes with the transform component in the scene
    // PxU32 nbActiveActors;
    // PxActor** activeActors(state.pActiveScene->getActiveActors(nbActiveActors));
    // for (PxU32 i = 0; i < nbActiveActors; i++)
    // {
    //     auto actor = (PxRigidActor*) activeActors[i];
    //     auto id = (EntityID) (long long) (actor->userData);
    //     auto eid = (EntityID) event.entity;

    //     // Find the entity and remove it from the physics simulation
    //     if (eid == id)
    //     {
    //         state.pActiveScene->removeActor(*actor);
    //         break;
    //     }
    // }
}

// TODO: This should also happen on rigidbody or collider *changes*
void OnRigidBodyCreate(const ComponentCreateEvent<CRigidBody> &event)
{
    // Entity entity = event.entity;
    // auto&& [rb, transform] = entity.GetComponent<CRigidBody, CTransform>();

    // PhysicsObjectInfo info;
    // info.Gravity = rb.Gravity;
    // info.IsKinematic = rb.IsKinematic;

    // // Material: Defaults are fine for now
    // // TODO: Create a physics material asset and pass as an optional CRigidBody field

    // // Setup the collider
    // PhysicsColliderInfo colliderInfo;

    // info.Collider = &colliderInfo;
    // if (entity.HasComponent<CSphereCollider>())
    // {
    //     auto collider = entity.GetComponent<CSphereCollider>();
    //     colliderInfo.Position = collider.Position + transform.Position;
    //     colliderInfo.Rotation = transform.Rotation;

    //     const PxSphereGeometry geom(collider.Radius);
    //     rb.detail.actor = createRigidBody(info, geom);

    //     // TODO: Enable/Disable can be handled here, but for a multiplier, I'll have to implement gravity myself
    //     if (info.Gravity < 0.2f)
    //     {
    //         rb.detail.actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
    //     }

    //     // NOTE: Entity IDs are just uint16_t's, so I cast that as a void* and store it with the object
    //     rb.detail.actor->userData = (void*) (EntityID) entity;
    //     state.pActiveScene->addActor(*rb.detail.actor);
    // }
    // else if (entity.HasComponent<CBoxCollider>())
    // {
    //     auto collider = entity.GetComponent<CBoxCollider>();
    //     colliderInfo.Position = collider.Position + transform.Position;
    //     colliderInfo.Rotation = transform.Rotation;

    //     const PxBoxGeometry geom(collider.Dimensions.x, collider.Dimensions.y, collider.Dimensions.z);
    //     rb.detail.actor = createRigidBody(info, geom);

    //     // TODO: Enable/Disable can be handled here, but for a multiplier, I'll have to implement gravity myself
    //     if (info.Gravity < 0.2f)
    //     {
    //         rb.detail.actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
    //     }

    //     rb.detail.actor->userData = (void*) (EntityID) entity;
    //     state.pActiveScene->addActor(*rb.detail.actor);
    // }
    // else if (entity.HasComponent<CCapsuleCollider>())
    // {
    //     auto collider = entity.GetComponent<CCapsuleCollider>();
    //     colliderInfo.Position = collider.Position + transform.Position;
    //     colliderInfo.Rotation = transform.Rotation;

    //     const PxCapsuleGeometry geom(collider.Radius, collider.Height);
    //     rb.detail.actor = createRigidBody(info, geom);

    //     // TODO: Enable/Disable can be handled here, but for a multiplier, I'll have to implement gravity myself
    //     if (info.Gravity < 0.2f)
    //     {
    //         rb.detail.actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
    //     }

    //     rb.detail.actor->userData = (void*) (EntityID) entity;
    //     state.pActiveScene->addActor(*rb.detail.actor);
    // }
    // else
    // {
    //     LOG_ERROR("Failed to add rigidbody to scene");
    //     throw std::invalid_argument("Invalid physics collider component");
    // }
}

void createScene(const PhysicsSceneInfo &info)
{
    LOG_INFO("Creating physics scene");
    state.pActiveScene = new World();
    state.pActiveScene->setGravity(info.Gravity);
}

void OnInit(const InitEvent &event)
{
    LOG_INFO("Creating physics world");
}

void OnSceneLoad(const SceneLoadEvent &event)
{
    // TODO: Get from the SceneManager
    PhysicsSceneInfo info{};
    createScene(info);
    state.pEngineScene = event.pScene;
    event.pScene->Observe(state.preUpdateObserver, entt::collector.update<CTransform>().where<CRigidBody>());
}

void OnSimulationBegin(const SimulationBeginEvent &event)
{
    state.isActive = true;
}

void OnSimulationPause(const SimulationPauseEvent &event)
{
    state.isActive = false;
}

void OnSimulationEnd(const SimulationEndEvent &event)
{
    state.isActive = false;
    // TODO: Restore state here or in pre-update?
}

void OnPreUpdate(const PreUpdateEvent &event)
{
    // This should happen even if moved from the editor
    // if (!state.isActive) return;

    // Sync physics changes with the transform component in the scene
    // state.preUpdateObserver.ForEach([](Entity entity) {
    //     auto&& [rb, tf] = entity.GetComponent<CRigidBody, CTransform>();

    //     // TODO: Set rotation and scale too
    //     // TODO: If statements for each collider shape, add those offsets to the transform
    //     PxTransform pt {};
    //     pt.p = toPxVec3(tf.Position);
    //     pt.q = rb.detail.actor->getGlobalPose().q;

    //     // Set CRigidBody directly because we do not want changes to detail to trigger ComponentUpdateEvent
    //     rb.detail.actor->setGlobalPose(pt);
    // });
}

void OnUpdate(const UpdateEvent &event)
{
    if (state.isActive) {
        state.pActiveScene->step(event.DeltaTime);
    }
}

void OnPostUpdate(const PostUpdateEvent &event)
{
    if (!state.isActive)
        return;

    // Sync physics changes with the transform component in the scene
    // PxU32 nbActiveActors;
    // PxActor** activeActors(state.pActiveScene->getActiveActors(nbActiveActors));
    // auto* registry = state.pEngineScene->GetRegistry();

    // for (PxU32 i = 0; i < nbActiveActors; i++)
    // {
    //     try
    //     {
    //         auto actor = (PxRigidActor*) activeActors[i];
    //         auto id = (EntityID) (long long) (actor->userData);

    //         // TODO: Change CTransform to hold a 4x4 matrix
    //         if (registry->valid(id))
    //         {
    //             auto& transform = registry->get<CTransform>(id);
    //             auto position = actor->getGlobalPose().p;

    //             // TODO: Should this trigger ComponentUpdateEvent?
    //             transform.Position = toVector3f(position);
    //         }
    //     }
    //     catch (const std::exception& e)
    //     {
    //         LOG_WARN("Failed to update physics state. {}", e.what());
    //     }
    // }
}

void OnDestroy(const DestroyEvent &event)
{
    LOG_INFO("Destroying Physics...");
    delete state.pActiveScene;
}

void ApplyForce(const CRigidBody &rb, Vector3f force, EForceType type)
{
    // using namespace physx;
    // auto* body = reinterpret_cast<PxRigidBody*>(rb.detail.actor);
    // auto pxforce = toPxVec3(force);
    // switch (type)
    // {
    //     case EForceType::IMPULSE:
    //         body->addForce(pxforce, PxForceMode::eIMPULSE);
    //         break;
    //     case EForceType::FORCE:
    //         body->addForce(pxforce, PxForceMode::eFORCE);
    //         break;
    // }
}
} // namespace Lotus::Physics
