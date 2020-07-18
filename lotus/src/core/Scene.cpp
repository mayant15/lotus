#include "lotus/scene.h"

std::shared_ptr<Camera> Scene::getCamera() const
{
    return camera;
}

std::vector<std::shared_ptr<Actor>> Scene::getActors() const
{
    return actors;
}

std::vector<std::shared_ptr<ALight>> Scene::getLights() const
{
    return lights;
}

void Scene::addCamera(glm::vec3 posVec, glm::vec3 worldUpVec)
{
    camera = std::make_shared<Camera>(posVec, worldUpVec);
}

void Scene::addActor(glm::vec3 position_, const CMesh& mesh)
{
    std::shared_ptr<Actor> actor = std::make_shared<Actor>(position_, mesh);
    actors.push_back(actor);
}

void Scene::addLight(glm::vec3 position, const CMesh& meshComponent)
{
    std::shared_ptr<ALight> light = std::make_shared<ALight>(position, meshComponent);
    lights.push_back(light);
}
