#include <lotus/ecs/ComponentRegistry.h>
#include <lotus/ecs/Entity.h>
#include <lotus/ecs/components/CCamera.h>
#include <lotus/physics/components.h>
#include <lotus/rendering/CMeshRenderer.h>
#include <lotus/resources/HDRI.h>

namespace Lotus
{
    std::unordered_map<entt::id_type, ComponentInfo> component_info;
    std::unordered_map<std::string, entt::id_type> str_to_id;

    void RegisterEngineComponents()
    {
        RegisterComponent<CCamera>();
        RegisterComponent<CTransform>();
        RegisterComponent<CSunLight>();
        RegisterComponent<CPointLight>();
        RegisterComponent<CSpotlight>();
        RegisterComponent<CSphereCollider>();
        RegisterComponent<CBoxCollider>();
        RegisterComponent<CCapsuleCollider>();
        RegisterComponent<CRigidBody>();
        RegisterComponent<CMeshRenderer>();
        RegisterComponent<CSkybox>();
        RegisterComponent<CDisplayName>();
    }
}
