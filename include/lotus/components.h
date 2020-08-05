#pragma once

#include "physics.h"
#include "lcommon.h"

namespace Lotus
{
    struct CTransform
    {
        // TODO: This should be just a mat4?
        Vector3f position = Vector3f(0.0f, 0.0f, 0.0f);
        // In Degrees
        Vector3f rotation = Vector3f(0.0f, 0.0f, 0.0f);
        Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f);
    };

    struct CScript
    {
        std::string path;
    };

    struct CCamera
    {
        bool IsActive = false;
        float FOV = 45;
    };

    struct CLight
    {
        Vector3f ambient = Vector3f(1.0f);
        Vector3f diffuse = Vector3f(1.0f);
        Vector3f specular = Vector3f(1.0f);
    };

    struct CPointLight : CLight
    {
        Vector3f position = Vector3f(0.0f);
        float constant = 1.0f;
        float linear = 1.0f;
        float quadratic = 1.0f;
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
        float linear = 1.0f;
        float quadratic = 1.0f;
        float innerCutOff = 1.0f;
        float outerCutOff = 1.0f;
    };
}
