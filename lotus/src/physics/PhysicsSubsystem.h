#pragma once

#include "lotus/physics/physics.h"
#include "lotus/physics/components.h"
#include "lotus/ILifecycle.h"
#include "lotus/ecs/Entity.h"

#include "physics/PxImpl.h"

namespace Lotus::Physics
{
    /**
     * @brief Options to configure physics properties of a scene.
     *
     * This class is a wrapper over properties provided by PhysX that can be used to
     * configure its physics scenes.
    */
    struct PhysicsSceneInfo
    {
        /**
         * @brief Gravitational acceleration for the scene
        */
        Vector3f Gravity { 0.0f, -9.81f, 0.0f };
    };

    struct PhysicsMaterialInfo
    {
        /**
         * @brief Coefficient of static friction. Must be between 0 and 1.
        */
        float StaticFriction = 0.7f;

        /**
         * @brief Coefficient of dynamic friction. Must be between 0 and 1.
         * This is less than or equal to the coefficient of static friction in real life.
        */
        float DynamicFriction = 0.7f;

        /**
         * @brief Coefficient of restitution. Must be between 0 and 1.
         * This represents the energy retained after a collision.
        */
        float Restitution = 0.8f;

        /**
         * @brief The density of the material.
        */
        float Density = 1.0f;
    };

    struct PhysicsColliderInfo
    {
        Vector3f Position = Vector3f(0.0f);
    };

    struct PhysicsSphereColliderInfo : PhysicsColliderInfo
    {
        float Radius = 1.0f;
    };

    struct PhysicsCapsuleColliderInfo : PhysicsColliderInfo
    {
        float Radius = 0.5f;
        float Height = 1.0f;
    };

    struct PhysicsBoxColliderInfo : PhysicsColliderInfo
    {
        Vector3f Dimensions = Vector3f (1.0f);
    };

    struct PhysicsObjectInfo
    {
        /**
         * @brief Gravity multiplier for this object.
        */
        float Gravity = 1.0f;

        /**
         * @brief Specify if the body is a kinematic object.
        */
        bool IsKinematic = false;

        /**
         * @brief Physics material used by this object.
        */
        PhysicsMaterialInfo Material;

        /**
         * @brief Reference to the collider properties for this object.
        */
        PhysicsColliderInfo* Collider;
    };

    using namespace physx;

    /**
     * @brief Class that wraps the entire physics subsystem.
     *
     * The physics subsystem runs simulations independently from other subsystems,
     * like rendering. This class maintains overall state of the physics world and
     * provides methods that can be used by other subsystems to interact with the
     * physics world.
    */
    class PhysicsSubsystem : public ILifecycle
    {
        bool _usePVD = true;
        PxDefaultAllocator _allocator;
        PhysXErrorCallback _errorCallback;
        PxFoundation* _pFoundation = nullptr;
        PxPhysics* _pPhysics = nullptr;
        PxDefaultCpuDispatcher* _pDispatcher = nullptr;
        PxPvd* _pPVD = nullptr;
        PxScene* _pActiveScene = nullptr;

    public:
        /**
         * @brief Create and intialize the subsystem object.
         * @param usePVD Whether to connect to PhysX Visual Debugger. Will not work in release builds.
        */
        explicit PhysicsSubsystem(bool usePVD = true) : _usePVD(usePVD) {}

        void OnRigidbodyCreate(const ComponentCreateEvent<CRigidBody>& event);

        void OnInit(const InitEvent& event) override;

        void OnBegin(const BeginEvent& event) override;

        void OnPreUpdate(const PreUpdateEvent& event) override;

        void OnUpdate(const UpdateEvent& event) override;

        void OnPostUpdate(const PostUpdateEvent& event) override;

        void OnPreDestroy(const PreDestroyEvent& event) override;

        void OnDestroy(const DestroyEvent& event) override;

        void OnShutdown(const ShutdownEvent& event) override;

    private:
        PxRigidActor* createRigidBody(const PhysicsObjectInfo& info, const PxGeometry& geometry) const;

        /**
         * @brief Create a new physics scene. These scenes are independent from the ones that
         * are rendered, and are used by PhysX internally for simulation.
         * @param info Config for the scene's physical properties.
        */
        void createScene(const PhysicsSceneInfo& info);
    };
}
