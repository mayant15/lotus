#pragma once

#include "lotus/physics/physics.h"
#include "lotus/ecs/ComponentRegistry.h"

namespace Lotus
{
    struct CLight
    {
        Vector3f ambient = Vector3f(0.0f);
        Vector3f diffuse = Vector3f(1.0f);
        Vector3f specular = Vector3f(0.4f);

        REGISTER_DECL();
    };
    SERIALIZE(CLight, ambient, diffuse, specular);
    REGISTER_BODY(CLight);

    struct CPointLight : CLight
    {
        float constant = 1.0f;
        float linear = 0.22f;
        float quadratic = 0.20f;

        REGISTER_DECL();
    };
    SERIALIZE(CPointLight, ambient, diffuse, specular, constant, linear, quadratic);
    REGISTER_BODY(CPointLight);

    struct CSpotlight : CLight
    {
        float constant = 1.0f;
        float linear = 0.07f;
        float quadratic = 0.017f;
        float innerCutOff = 12.5f;
        float outerCutOff = 17.5f;

        REGISTER_DECL();
    };
    // TODO: Better serialize nested components so that I don't have to write this huge list
    SERIALIZE(CSpotlight, ambient, diffuse, specular, constant, linear, quadratic, innerCutOff, outerCutOff);
    REGISTER_BODY(CSpotlight);
}
