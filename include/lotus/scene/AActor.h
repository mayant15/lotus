#pragma once

#include "lotus/components/scene.h"
#include "Scene.h"
#include "lotus/physics.h"

namespace Lotus
{
    // TODO: An entity that is placed in the scene, i.e. entity with a transform, plus some helper functions
    class AActor : public Entity
    {
    public:
        AActor() : AActor(NULL_ENTITY, nullptr) {}
        AActor(EntityID id) : AActor(id, nullptr) {}
        AActor(EntityID id, Scene* scene);

        CTransform& GetTransform();

        Vector3f GetAbsolutePosition();
        Vector3f GetAbsoluteRotation();
        Vector3f GetAbsoluteScale();

        Vector3f GetRightVector();
        Vector3f GetForwardVector();
        Vector3f GetUpVector();
    };
}
