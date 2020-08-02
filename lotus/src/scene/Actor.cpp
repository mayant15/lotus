#include "lotus/scene.h"

namespace Lotus
{

    ALight::ALight(const CSpotlight& light_) : AActor(light_.position)
    {
        light = light_;
    }

    AActor::AActor(Vector3f position_)
    {
        transform.position = position_;
    }

    void AActor::update()
    {
        for (const std::shared_ptr<AActor>& child : children)
        {
            child->update();
        }
    }

    void AActor::start()
    {
        for (const std::shared_ptr<AActor>& child : children)
        {
            child->start();
        }
    }

    void AActor::addChild(const std::shared_ptr<AActor>& child)
    {
        children.push_back(child);
    }
}
