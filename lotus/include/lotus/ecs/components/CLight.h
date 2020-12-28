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

        REGISTER_DECL();
    };
    SERIALIZE(CSunLight, direction, color);
    REGISTER_BODY(CSunLight);

    struct CPointLight
    {
        Color color = COLOR_WHITE;
        float constant = 1.0f;
        float linear = 0.22f;
        float quadratic = 0.20f;

        REGISTER_DECL();
    };
    SERIALIZE(CPointLight, color, constant, linear, quadratic);
    REGISTER_BODY(CPointLight);

    struct CSpotlight
    {
        Color color = COLOR_WHITE;
        float constant = 1.0f;
        float linear = 0.07f;
        float quadratic = 0.017f;
        float innerCutOff = 12.5f;
        float outerCutOff = 17.5f;

        REGISTER_DECL();
    };
    SERIALIZE(CSpotlight, color, constant, linear, quadratic, innerCutOff, outerCutOff);
    REGISTER_BODY(CSpotlight);
}
