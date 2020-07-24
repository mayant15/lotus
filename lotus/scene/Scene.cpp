#include "lotus/scene.h"

namespace Lotus
{
    void Scene::addCamera(SRefCamera& camera_)
    {
        camera = camera_;
    }

    void Scene::addActor(const SRefActor& actor)
    {
        actors.push_back(actor);
    }

    void Scene::addLight(const SRefALight& light)
    {
        lights.push_back(light);
        actors.push_back(light);
    }

    const SRefCamera& Scene::getCamera() const
    {
        return camera;
    }

    const std::vector<SRefActor>& Scene::getActors() const
    {
        return actors;
    }

    const std::vector<SRefALight>& Scene::getLights() const
    {
        return lights;
    }
}
