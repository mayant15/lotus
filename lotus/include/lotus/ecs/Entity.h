#pragma once

#include "lotus/ecs/Recipe.h"

namespace Lotus
{
    /**
     * @brief An entity handle, essentially just an int.
    */
    typedef entt::entity EntityID;

    constexpr auto NULL_ENTITY = entt::null;

    /**
    * @brief Lightweight wrapper class to represent entities.
    *
    * This class is the main interface through which the user should manipulate entities.
    * It provides methods to manipulate components and destroy the entity. The class wraps
    * an entity handle and a registry pointer, and is therefore lightweight enough to be passed
    * around.
   */
    class LOTUS_API Entity
    {
        EntityID _id = entt::null;
        entt::registry* _registry = nullptr;

    public:
        Entity(EntityID id, entt::registry* registry) : _id(id), _registry(registry) {}

        /**
         * @brief Overload this operator to allow casts to EntityID.
        */
        explicit operator EntityID() const
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
            return _registry->emplace<Component>(_id, std::forward<Args>(args)...);
        }

        /**
         * @brief Get components from this entity
         * @tparam Components Types of the components to get
         * @return Tuple with all components
        */
        template<typename... Components>
        decltype(auto) GetComponent()
        {
            return _registry->get<Components...>(_id);
        }

        /**
         * @brief Destroy this entity and remove it from the scene
        */
        // ReSharper disable once CppMemberFunctionMayBeConst
        void Destroy()
        {
            _registry->destroy(_id);
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

    void ECSInitialize();
    void ECSShutdown();

    LOTUS_API entt::registry* GetRegistry();
    LOTUS_API Entity CreateEntity();
    LOTUS_API Entity CreateEntity(const Recipe& recipe);
}