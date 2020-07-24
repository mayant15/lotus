#include "lotus/scene.h"

namespace Lotus
{

    ALight::ALight(const CSpotlight& light_, const Resource::SRefModel& model_, const Rendering::SRefShader& shader_)
            : Actor(light_.position, model_, shader_)
    {
        light = light_;
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

