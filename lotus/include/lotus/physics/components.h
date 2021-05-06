#pragma once

#include <lotus/ecs/ComponentRegistry.h>
#include <lotus/physics/Vector.h>

namespace physx
{
    class PxRigidActor;
}

namespace Lotus
{
    struct CSphereCollider
    {
        /** @brief Center of the sphere */
        Vector3f Position {0.0f};

        /** @brief Radius of the sphere */
        float Radius = 1.0f;

        GENERATED_BODY(CSphereCollider, Position, Radius);
    };

    struct CBoxCollider
    {
        /** @brief Center of the box */
        Vector3f Position {0.0f};

        /** @brief Dimensions of the box on local X, Y, Z */
        Vector3f Dimensions {1.0f};

        GENERATED_BODY(CBoxCollider, Position, Dimensions);
    };

    struct CCapsuleCollider
    {
        /** @brief Center of the capsule */
        Vector3f Position {0.0f};

        /** @brief Radius of the capsule */
        float Radius = 1.0f;

        /** @brief Height of the capsule */
        float Height = 1.0f;

        GENERATED_BODY(CCapsuleCollider, Position, Radius, Height);
    };

    /** @attention You must add a collider and a transform before adding a rigidbody component */
    struct CRigidBody
    {
        float Gravity = 1.0f;
        bool IsKinematic = false;

        GENERATED_BODY(CRigidBody, Gravity, IsKinematic);

        struct
        {
            physx::PxRigidActor* actor = nullptr;
        } detail;
    };
}
