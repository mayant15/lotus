#pragma once

#include "lotus/physics.h"
#include "lotus/lotus_export.h"
#include <string>

namespace Lotus
{
    struct LOTUS_API CTransform
    {
        Vector3f Position {0.0f,  0.0f,  0.0f};
        Vector3f Rotation {0.0f, -90.0f, 0.0f};
        Vector3f Scale    {1.0f,  1.0f,  1.0f};
    };

    struct LOTUS_API CScript
    {
        std::string Path;
    };

    struct LOTUS_API CCamera
    {
        bool IsActive = false;
        float FOV = 45;
    };
}
