#pragma once

#include "lotus/physics.h"

namespace Lotus
{
    struct CLight
    {
        Vector3f ambient = Vector3f(0.2f);
        Vector3f diffuse = Vector3f(1.0f);
        Vector3f specular = Vector3f(0.4f);
    };

    struct CPointLight : CLight
    {
        Vector3f position = Vector3f(0.0f);
        float constant = 1.0f;
        float linear = 0.22f;
        float quadratic = 0.20f;
    };

    struct CDirectionalLight : CLight
    {
        Vector3f direction = Vector3f(0.0f);
    };

    struct CSpotlight : CLight
    {
        Vector3f position = Vector3f(0.0f);
        Vector3f direction = Vector3f(0.0f);
        float constant = 1.0f;
        float linear = 0.07f;
        float quadratic = 0.017f;
        float innerCutOff = 12.5f;
        float outerCutOff = 17.5f;
    };
}
