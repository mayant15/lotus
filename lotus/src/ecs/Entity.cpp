#include "lotus/ecs/Entity.h"

namespace Lotus
{
    static URef<entt::registry> pRegistry = nullptr;

    void ECSInitialize()
    {
        pRegistry = std::make_unique<entt::registry>();
    }

    void ECSShutdown()
    {
        pRegistry.reset();
    }

    entt::registry* GetRegistry()
    {
        return pRegistry.get();
    }

    Entity CreateEntity()
    {
        EntityID id = pRegistry->create();
        return {id, pRegistry.get()};
    }

    Entity CreateEntity(const Recipe &recipe)
    {
        // Create empty
        EntityID id = pRegistry->create();

        for (auto component : recipe)
        {
            // TODO: Deserialize the component and add it to the entity
            // TODO: Make recipies resources and handle them through the AssetRegistry
            auto type_id = entt::resolve_id(entt::hashed_string {component.Name.c_str()});
        }

        return {id, pRegistry.get()};
    }
}
