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

    void Scene::addLight(const SRefALight& light, ELight type) {
        actors.push_back(light);
        switch (type)
        {
            case ELight::SPOT:
                spotlights.push_back(light);
                break;
            case ELight::DIR:
                dirLights.push_back(light);
                break;
            case ELight::POINT:
                pointLights.push_back(light);
                break;
        }
    }

    std::vector<CPointLight> Scene::getPointLightProps() const
    {
        std::vector<CPointLight> v;
        for (const SRefALight& light : pointLights)
        {
            CPointLight props;
            props.position = light->light.position;
            props.constant = light->light.constant;
            props.linear = light->light.linear;
            props.quadratic = light->light.quadratic;
            props.specular = light->light.specular;
            props.diffuse = light->light.diffuse;
            props.ambient = light->light.ambient;
            v.push_back(props);
        }
        return v;
    }

    std::vector<CSpotlight> Scene::getSpotlightProps() const
    {
        std::vector<CSpotlight> v;
        for (const SRefALight& light : spotlights)
        {
            CSpotlight props;
            props.position = light->light.position;
            props.direction = light->light.direction;
            props.innerCutOff = light->light.innerCutOff;
            props.outerCutOff = light->light.outerCutOff;
            props.constant = light->light.constant;
            props.linear = light->light.linear;
            props.quadratic = light->light.quadratic;
            props.specular = light->light.specular;
            props.diffuse = light->light.diffuse;
            props.ambient = light->light.ambient;
            v.push_back(props);
        }
        return v;
    }

    std::vector<CDirectionalLight> Scene::getDirLightProps() const
    {
        std::vector<CDirectionalLight> v;
        for (const SRefALight& light : dirLights)
        {
            CDirectionalLight props;
            props.direction = light->light.direction;
            props.specular = light->light.specular;
            props.diffuse = light->light.diffuse;
            props.ambient = light->light.ambient;
            v.push_back(props);
        }
        return v;
    }
}
