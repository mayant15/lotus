#pragma once

#include <lotus/physics/Vector.h>

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
        /** @brief Gravitational acceleration for the scene. */
        Vector3f Gravity { 0.0f, -9.81f, 0.0f };
    };

    struct PhysicsMaterialInfo
    {
        /** @brief Coefficient of static friction. Must be between 0 and 1. */
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
        float Restitution = 1.0f;

        /** @brief The density of the material. */
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
        /** @brief Gravity multiplier for this object. */
        float Gravity = 1.0f;

        /** @brief Specify if the body is a kinematic object. */
        bool IsKinematic = false;

        /** @brief Physics material used by this object. */
        PhysicsMaterialInfo Material;

        /** @brief Reference to the collider properties for this object. */
        PhysicsColliderInfo* Collider;
    };
}
