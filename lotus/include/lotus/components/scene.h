#pragma once

#include "lotus/physics.h"
#include <string>

namespace Lotus
{
    struct CTransform
    {
        Vector3f Position {0.0f,  0.0f,  0.0f};
        Vector3f Rotation {0.0f, -90.0f, 0.0f};
        Vector3f Scale    {1.0f,  1.0f,  1.0f};
    };

    struct CScript
    {
        std::string Path;
    };

    struct CCamera
    {
        bool IsActive = false;
        float FOV = 45;
    };
}
