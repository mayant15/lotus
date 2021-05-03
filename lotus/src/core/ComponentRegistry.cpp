#include <lotus/ecs/ComponentRegistry.h>
#include <lotus/ecs/Entity.h>
#include <lotus/ecs/components/CCamera.h>
#include <lotus/physics/components.h>
#include <lotus/rendering/CMeshRenderer.h>
#include <lotus/resources/HDRI.h>

namespace Lotus
{
    std::unordered_map<component_ctor_key_t, component_ctor_t> ctors;

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
    }
}
