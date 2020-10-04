#pragma once

#include "lotus/ecs/EntityRegistry.h"

namespace Lotus
{
    /**
    * @brief Lightweight wrapper class to represent entities.
    *
    * This class is the main interface through which the user should manipulate entities.
    * It provides methods to manipulate components and destroy the entity. The class wraps
    * an entity handle and a registry pointer, and is therefore lightweight enough to be passed
    * around.
    *
    * @note Creating this object is **not** the same as creating an entity with the registry.
    * If you want to instantiate a new entity, use Lotus::ECS::Create<Entity>() instead.
   */
    class LOTUS_API Entity
    {
        EntityID _id = entt::null;
        EntityRegistry* _registry = nullptr;

    public:
        Entity(EntityID id, EntityRegistry* registry) : _id(id), _registry(registry) {}

        /**
         * @brief Overload this operator to allow casts to EntityID.
        */
        operator EntityID() const
        {
            return _id;
        }

        /**
         * @brief Add a component to this entity
         * @tparam Component Type of the component
         * @tparam Args Types for optional parameters
         * @param args Optional parameters, used to create the component in-place
         * @return A reference to the newly created component
        */
        template<typename Component, typename ...Args>
        Component& AddComponent(Args&& ...args)
        {
            return _registry->AddComponentToEntity<Component>(_id, std::forward<Args>(args)...);
        }

        /**
         * @brief Get components from this entity
         * @tparam Components Types of the components to get
         * @return Tuple with all components
        */
        template<typename... Components>
        decltype(auto) GetComponent()
        {
            return _registry->GetComponentFromEntity<Components...>(_id);
        }

        /**
         * @brief Destroy this entity and remove it from the scene
        */
        // ReSharper disable once CppMemberFunctionMayBeConst
        void Destroy()
        {
            _registry->DestroyEntity(_id);
        }

        // TODO: Implement
        static std::vector<nlohmann::json> GetRecipe()
        {
            const nlohmann::json data = {
                    {"component_name", "value"}
            };

            return { data };
        }
    };
}