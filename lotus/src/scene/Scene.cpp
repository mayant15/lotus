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

    Entity Scene::CreateEntity(bool serialize)
    {
        EntityID id = _registry.create();
        if (serialize)
        {
            _registry.emplace<CSerialize>(id);
            _registry.emplace<CDisplayName>(id, "New Entity");
        }

        return { id, &_registry };
    }

    Entity Scene::CreateEntity(const std::string& path, bool serialize)
    {
        // Create empty
        EntityID id = _registry.create();
        if (serialize)
        {
            _registry.emplace<CSerialize>(id);
            _registry.emplace<CDisplayName>(id, "New Entity");
        }

        nlohmann::json data;
        std::ifstream infile(path);
        infile >> data;

        for (auto& el : data.items())
        {
            try
            {
                auto info = GetComponentInfo(el.key());
                info.assignFn(id, _registry, el.value());
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

    Entity Scene::GetEntity(EntityID entity)
    {
        if (_registry.valid(entity))
        {
            return Entity { entity, &_registry };
        }
        throw std::runtime_error { "Invalid entity ID" };
    }

    Entity Scene::GetEntity(const std::string& name)
    {
        auto view = _registry.view<CDisplayName>();
        for (const auto& e : view)
        {
            const auto& cd = view.get(e);
            if (cd.Name == name)
            {
                return Entity { e, &_registry };
            }
        }
        throw std::runtime_error { "No entity named " + name };
    }
}
