#pragma once

#include "lotus/physics/physics.h"

namespace Lotus
{
    struct CLight
    {
        Vector3f ambient = Vector3f(0.0f);
        Vector3f diffuse = Vector3f(1.0f);
        Vector3f specular = Vector3f(0.4f);
    };

    struct CPointLight : CLight
    {
        float constant = 1.0f;
        float linear = 0.22f;
        float quadratic = 0.20f;
    };

    struct CSpotlight : CLight
    {
        float constant = 1.0f;
        float linear = 0.07f;
        float quadratic = 0.017f;
        float innerCutOff = 12.5f;
        float outerCutOff = 17.5f;
    };
}
