#pragma once

#include "AActor.h"

namespace Lotus
{
    class ACamera : public AActor
    {
    public:
        ACamera() : ACamera(NULL_ENTITY, nullptr) {}
        ACamera(EntityID id) : ACamera(id, nullptr) {}
        ACamera(EntityID id, Scene* scene);

        Matrix4f GetViewMatrix();
        float GetFieldOfView();
    };
}