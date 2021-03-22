#pragma once

#include "lotus/ecs/ComponentRegistry.h"
#include "lotus/ecs/components/CTransform.h"

namespace Lotus
{
    struct CCamera
    {
        bool IsActive = false;
        float FOV = 45;
        GENERATED_BODY(CCamera, IsActive, FOV);
    };

    inline Matrix4f GetViewMatrix(const CTransform& transform)
    {
        return LLookAt(transform.Position, transform.Position + GetForwardVector(transform), GetUpVector(transform));
    }
}
