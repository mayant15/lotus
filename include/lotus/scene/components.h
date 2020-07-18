#pragma once
#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include "lotus/rendering/Model.h"

struct Component
{
    //
};

struct CTransform : Component
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct CMesh : Component
{
    std::shared_ptr<Model> model;
    std::shared_ptr<Shader> shader;

    CMesh() = default;
    CMesh(const Model& model_, const Shader& shader_);
};

struct CLight : Component
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct CPointLight : CLight
{
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct CDirectionalLight : CLight
{
    glm::vec3 direction;
};

struct CSpotlight : CLight
{
    glm::vec3 position;
    glm::vec3 direction;
    float constant;
    float linear;
    float quadratic;
    float innerCutOff;
    float outerCutOff;
};
