#pragma once

#include "AActor.h"

namespace Lotus
{
    class LOTUS_API ACamera : public AActor
    {
    public:
        ACamera(EntityID id = NULL_ENTITY, Scene* scene = nullptr) : AActor(id, scene)
        {}

        Matrix4f GetViewMatrix();

        float GetFieldOfView();
    };
}