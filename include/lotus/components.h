#pragma once

#include <glm/glm.hpp>
#include "core.h"

namespace Lotus
{
    struct CTransform : IComponent
    {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    };

    struct CLight : IComponent
    {
        glm::vec3 ambient = glm::vec3(1.0f);
        glm::vec3 diffuse = glm::vec3(1.0f);
        glm::vec3 specular = glm::vec3(1.0f);
    };

    struct CPointLight : CLight
    {
        glm::vec3 position = glm::vec3(0.0f);
        float constant = 1.0f;
        float linear = 1.0f;
        float quadratic = 1.0f;
    };

    struct CDirectionalLight : CLight
    {
        glm::vec3 direction = glm::vec3(0.0f);
    };

    struct CSpotlight : CLight
    {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 direction = glm::vec3(0.0f);
        float constant = 1.0f;
        float linear = 1.0f;
        float quadratic = 1.0f;
        float innerCutOff = 1.0f;
        float outerCutOff = 1.0f;
    };
}