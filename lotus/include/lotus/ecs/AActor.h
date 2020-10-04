#pragma once

#include "lotus/ecs/Entity.h"
#include "lotus/physics/physics.h"

namespace Lotus
{
    struct CTransform
    {
        Vector3f Position {0.0f,  0.0f,  0.0f};
        Vector3f Rotation {0.0f, -90.0f, 0.0f};
        Vector3f Scale    {1.0f,  1.0f,  1.0f};
    };

    LOTUS_API Vector3f GetRightVector(Entity actor);

    LOTUS_API Vector3f GetForwardVector(Entity actor);

    LOTUS_API Vector3f GetUpVector(Entity actor);
}
