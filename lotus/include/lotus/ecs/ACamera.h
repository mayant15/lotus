#pragma once

#include "lotus/ecs/AActor.h"

namespace Lotus
{
    struct CCamera
    {
        bool IsActive = false;
        float FOV = 45;
    };

//    LOTUS_API Matrix4f GetViewMatrix(Entity camera);
}
