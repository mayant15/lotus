#include <lotus/ecs/Entity.h>
#include <lotus/ecs/EventManager.h>
#include <lotus/physics/components.h>
#include <lotus/rendering/CMeshRenderer.h>
#include <lotus/debug.h>

namespace Lotus
{
    // TODO: Move this into a scene object
    entt::registry* pRegistry = nullptr;

    template<typename T>
    void dispatchEntityEvent(entt::registry& registry, entt::entity entity)
    {
        auto event = T {};
        event.entityID = entity;
        EventManager::Get().Dispatch(event);
    }

    void ECSInitialize()
    {
        pRegistry = new entt::registry;

        // TODO: Generalize for all components
        pRegistry->on_construct<CRigidBody>().connect<dispatchEntityEvent<ComponentCreateEvent<CRigidBody>>>();
        pRegistry->on_destroy<CRigidBody>().connect<dispatchEntityEvent<ComponentDestroyEvent<CRigidBody>>>();

        pRegistry->on_construct<CMeshRenderer>().connect<dispatchEntityEvent<ComponentCreateEvent<CMeshRenderer>>>();
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
        return { id, pRegistry };
    }

    Entity CreateEntity(const std::string& path)
    {
        // Create empty
        EntityID id = pRegistry->create();

        nlohmann::json data;
        std::ifstream infile(path);
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

        return { id, pRegistry };
    }
}
