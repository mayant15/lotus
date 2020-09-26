#pragma once

#include "lotus/components/scene.h"
#include "Scene.h"
#include "lotus/physics.h"

namespace Lotus
{
    // TODO: An entity that is placed in the scene, i.e. entity with a transform, plus some helper functions


    /**
     * @brief An entity that can be placed in the scene
     *
     * This actor class should be a lightweight wrapper to provide actor specific functionality to
     * the abstract concept of an entity. An object hierarchy can be created to provide functions.
    */
    class LOTUS_API AActor : public Entity
    {
    public:
        AActor(EntityID id = NULL_ENTITY, Scene* scene = nullptr) : Entity(id, scene)
        {}

        /**
         * @brief Get the transform for an actor
         * @return A reference to the CTransform on the actor
        */
        CTransform& GetTransform();

        /**
         * @brief Get the position of the actor in world coordinates
         * @return Vector3f for the absolute position
        */
        Vector3f GetAbsolutePosition();

        Vector3f GetAbsoluteRotation();

        Vector3f GetAbsoluteScale();

        Vector3f GetRightVector();

        Vector3f GetForwardVector();

        Vector3f GetUpVector();
    };
}
