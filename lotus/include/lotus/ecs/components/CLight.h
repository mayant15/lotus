#pragma once

#include <lotus/physics/Vector.h>
#include <lotus/rendering/common.h>
#include <lotus/ecs/ComponentRegistry.h>

namespace Lotus
{
    struct CSunLight
    {
        Color color = COLOR_WHITE;
        Vector3f direction = Vector3f(1.0f);

        GENERATED_BODY(CSunLight, direction, color);
    };

    struct CPointLight
    {
        Color color = COLOR_WHITE;
        float constant = 1.0f;
        float linear = 0.22f;
        float quadratic = 0.20f;

        GENERATED_BODY(CPointLight, color, constant, linear, quadratic);
    };

    struct CSpotlight
    {
        Color color = COLOR_WHITE;
        float constant = 1.0f;
        float linear = 0.07f;
        float quadratic = 0.017f;
        float innerCutOff = 12.5f;
        float outerCutOff = 17.5f;

        GENERATED_BODY(CSpotlight, color, constant, linear, quadratic, innerCutOff, outerCutOff);
    };
}
