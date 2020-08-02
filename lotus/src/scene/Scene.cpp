#include "lotus/scene.h"

namespace Lotus
{
    Scene::Scene() {
        root = std::make_shared<AActor>(ORIGIN);
    }

    void Scene::update()
    {
        root->update();
    }

    SRefAActor Scene::getRoot() const
    {
        return root;
    }

//    std::vector<CPointLight> Scene::getPointLightProps() const
//    {
//        std::vector<CPointLight> v;
//        for (const SRefALight& light : pointLights)
//        {
//            CPointLight props;
//            props.position = light->light.position;
//            props.constant = light->light.constant;
//            props.linear = light->light.linear;
//            props.quadratic = light->light.quadratic;
//            props.specular = light->light.specular;
//            props.diffuse = light->light.diffuse;
//            props.ambient = light->light.ambient;
//            v.push_back(props);
//        }
//        return v;
//    }
//
//    std::vector<CSpotlight> Scene::getSpotlightProps() const
//    {
//        std::vector<CSpotlight> v;
//        for (const SRefALight& light : spotlights)
//        {
//            CSpotlight props;
//            props.position = light->light.position;
//            props.direction = light->light.direction;
//            props.innerCutOff = light->light.innerCutOff;
//            props.outerCutOff = light->light.outerCutOff;
//            props.constant = light->light.constant;
//            props.linear = light->light.linear;
//            props.quadratic = light->light.quadratic;
//            props.specular = light->light.specular;
//            props.diffuse = light->light.diffuse;
//            props.ambient = light->light.ambient;
//            v.push_back(props);
//        }
//        return v;
//    }
//
//    std::vector<CDirectionalLight> Scene::getDirLightProps() const
//    {
//        std::vector<CDirectionalLight> v;
//        for (const SRefALight& light : dirLights)
//        {
//            CDirectionalLight props;
//            props.direction = light->light.direction;
//            props.specular = light->light.specular;
//            props.diffuse = light->light.diffuse;
//            props.ambient = light->light.ambient;
//            v.push_back(props);
//        }
//        return v;
//    }
}
