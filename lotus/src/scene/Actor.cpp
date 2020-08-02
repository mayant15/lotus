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

    // TODO:
    void AActor::update()
    {

    }

    // TODO:
    void AActor::start()
    {

    }
}
