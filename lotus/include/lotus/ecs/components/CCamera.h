#pragma once

#include "lotus/ecs/ComponentRegistry.h"
#include "lotus/ecs/components/CTransform.h"

namespace Lotus
{
    struct CCamera
    {
        bool IsActive = false;
        float FOV = 45;

        REGISTER_DECL();
    };
    SERIALIZE(CCamera, IsActive, FOV);
    REGISTER_BODY(CCamera);

    inline Matrix4f GetViewMatrix(const CTransform& transform)
    {
        return LLookAt(transform.Position, transform.Position + GetForwardVector(transform), GetUpVector(transform));
    }
}
