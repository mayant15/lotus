#include "lotus/physics/physics.h"
#include "lotus/ecs/Entity.h"
#include "lotus/ecs/EventManager.h"

namespace Lotus
{
    entt::registry* pRegistry = nullptr;

    template <typename T>
    void dispatchComponentCreateEvent(entt::registry& registry, entt::entity entity)
    {
        auto event = ComponentCreateEvent<T> {};
        event.entityID = entity;
        EventManager::Get().Dispatch(event);
    }

    void ECSInitialize()
    {
        pRegistry = new entt::registry;

        // TODO: Generalize for all components
        pRegistry->on_construct<CRigidBody>().connect<dispatchComponentCreateEvent<CRigidBody>>();
    }

    void ECSShutdown()
    {
        delete pRegistry;
    }

    entt::registry* GetRegistry()
    {
        return pRegistry;
    }

    Entity CreateEntity()
    {
        EntityID id = pRegistry->create();
        return {id, pRegistry};
    }

    Entity CreateEntity(const Recipe &recipe)
    {
        // Create empty
        EntityID id = pRegistry->create();

        for (const auto& component : recipe)
        {
            // TODO: Deserialize the component and add it to the entity
            // TODO: Make recipies resources and handle them through the AssetRegistry
//            auto type_id = entt::resolve_id(entt::hashed_string {component.Name.c_str()});
        }

        return {id, pRegistry};
    }
}
