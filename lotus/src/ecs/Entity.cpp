#include "lotus/ecs/Entity.h"

#include "lotus/ecs/EventManager.h"

#include "lotus/physics/components.h"

#include <fstream>
#include <lotus/debug.h>

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

    Entity CreateEntity(const std::string& path)
    {
        // Create empty
        EntityID id = pRegistry->create();

        nlohmann::json data;
        std::ifstream infile (path);
        infile >> data;

        for (auto& el : data.items())
        {
            try
            {
                auto ct = GET_COMPONENT_CTOR (el.key());
                ct(id, *pRegistry, el.value());
            }
            catch (const std::exception& e)
            {
                LOG_WARN("Invalid component: {}", el.key());
            }
        }

        return {id, pRegistry};
    }
}
