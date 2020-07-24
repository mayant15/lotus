#include "lotus/scene.h"

namespace Lotus
{

    ALight::ALight(glm::vec3 position_, const Resource::SRefModel& model_, const Rendering::SRefShader& shader_)
            : Actor(position_, model_, shader_)
    {
        light.position = position_;
        light.diffuse = glm::vec3(1.0f);
        light.ambient = glm::vec3(0.2f);
        light.specular = glm::vec3(0.5f);
        light.constant = 1.0f;
        light.linear = 0.09f;
        light.quadratic = 0.032f;
    }

    Actor::Actor(glm::vec3 position_, const Resource::SRefModel& model_, const Rendering::SRefShader& shader_)
    {
        transform.position = position_;
        model.model = model_;
        model.shader = shader_;
    }

    // TODO:
    void Actor::update()
    {

    }

    // TODO:
    void Actor::start()
    {

    }
}

