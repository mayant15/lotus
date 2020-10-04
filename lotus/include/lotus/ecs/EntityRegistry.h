#pragma once

#include "lotus/internal/entt/entt.hpp"
#include "lotus/lcommon.h"

namespace Lotus
{
    /**
     * @brief An entity handle, essentially just an int.
    */
    typedef entt::entity EntityID;

    constexpr auto NULL_ENTITY = entt::null;

    /**
     * @brief Singleton registry class that actually manages entities.
    */
    class EntityRegistry : public Singleton<EntityRegistry>
    {
        entt::registry _registry;

    public:
        template<typename Component, typename... Args>
        Component& AddComponentToEntity(EntityID id, Args&&... args)
        {
            return _registry.emplace<Component>(id, std::forward<Args>(args)...);
        }

        template<typename... Components>
        decltype(auto) GetComponentFromEntity(EntityID id)
        {
            return _registry.get<Components...>(id);
        }

        void DestroyEntity(EntityID id)
        {
            _registry.destroy(id);
        }

    private:
        EntityRegistry() = default;
        friend Singleton<EntityRegistry>;

        template<typename... Component>
        friend auto FindEntitiesByComponent();

        template<typename T>
        friend T CreateEntity();
    };

    // TODO: This view should have my entity class, not EntityID
    // OR, this view could return a const reference to the registry as well
    template<typename... Component>
    auto FindEntitiesByComponent()
    {
        return GET(EntityRegistry)._registry.view<Component...>();
    }

    template<typename T>
    T CreateEntity()
    {
        // Create empty
        auto pEntityRegistry = &GET(EntityRegistry);
        EntityID id = pEntityRegistry->_registry.create();

//        // Get components in recipe
//        auto components = T::GetRecipe();
//
//        // Add components to created entity
//        for (const auto& component : components)
//        {
//            // TODO: Deserialize the component and add it to the entity
//            // TODO: Make recipies resources and handle them through the AssetRegistry
//        }

        return { id, pEntityRegistry };
    }
}
