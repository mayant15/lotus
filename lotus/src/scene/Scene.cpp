#include <lotus/scene/Scene.h>
#include <lotus/ecs/ComponentRegistry.h>
#include <lotus/ecs/EventManager.h>
#include <lotus/physics/components.h>
#include <lotus/rendering/CMeshRenderer.h>
#include <lotus/debug.h>

namespace Lotus
{
    template<typename T>
    void dispatchEntityEvent(entt::registry& registry, entt::entity entity)
    {
        auto event = T {};
        event.entityID = entity;
        EventManager::Get().Dispatch(event);
    }

    Scene::Scene()
    {
        // TODO: Generalize for all components
        _registry.on_construct<CRigidBody>().connect<dispatchEntityEvent<ComponentCreateEvent<CRigidBody>>>();
        _registry.on_destroy<CRigidBody>().connect<dispatchEntityEvent<ComponentDestroyEvent<CRigidBody>>>();
        _registry.on_construct<CMeshRenderer>().connect<dispatchEntityEvent<ComponentCreateEvent<CMeshRenderer>>>();
    }

    Entity Scene::CreateEntity()
    {
        EntityID id = _registry.create();
        return { id, &_registry };
    }

    Entity Scene::CreateEntity(const std::string& path)
    {
        // Create empty
        EntityID id = _registry.create();

        nlohmann::json data;
        std::ifstream infile(path);
        infile >> data;

        for (auto& el : data.items())
        {
            try
            {
                auto ct = GetComponentCtor(el.key());
                ct(id, _registry, el.value());
            }
            catch (const std::exception& e)
            {
                LOG_WARN("Invalid component: {}", el.key());
            }
        }

        return { id, &_registry };
    }

    entt::registry* Scene::GetRegistry()
    {
        return &_registry;
    }
}
