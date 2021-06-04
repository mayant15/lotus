#pragma once


#include <lotus/lcommon.h>
#include <lotus/ecs/Entity.h>
#include <lotus/ecs/Event.h>

#include "SceneTreeNode.h"

namespace Lotus
{
    // TODO: Make this just the scene hierarchy
    struct LOTUS_API Scene
    {
        SceneTreeNode* Root = nullptr;

        struct {
            std::string path;
        } detail;

        Scene();

        EntityRegistry* GetRegistry();

        /**
         * @brief Fetch an entity in the scene by EntityID.
         *
         * If you know the ID for the entity of interest, this function can be used
         * to fetch it by that ID.
         *
         * @param entity ID of the entity to search for
         * @return Entity wrapper over the instance in the current scene
         * @throws std::runtime_error if the entity ID supplied is invalid
         */
        Entity GetEntity(EntityID entity);

        /**
         * @brief Fetch an entity in the scene by display name.
         *
         * If the entity that you are trying to find has a CDisplayName component,
         * this function can be used fetch it by the Name field of the component,
         * neatly wrapped in the Entity convenience class.
         *
         * @param name Name to search for among all CDisplayName components
         * @return Entity wrapper over the instance in the current scene
         * @throws std::runtime_error if no entity has a CDisplayName with the supplied name
         */
        Entity GetEntity(const std::string& name);

        Entity CreateEntity(bool serialize = true);
        Entity CreateEntity(const std::string& path, bool serialize = true);

        template <class Matcher>
        inline void Observe(Observer& observer, const Matcher& matcher)
        {
            observer.Connect(_registry, matcher);
        }

        template <class Matcher>
        inline Observer Observe(const Matcher& matcher)
        {
            return Observer { _registry, matcher };
        }

        // TODO: Wrapper over registry, view
        //   template<class ...Components>
        //   auto View() {}

    private:
        EntityRegistry _registry;
    };

    struct SceneLoadEvent
    {
        IMMEDIATE_EVENT();
        Scene* pScene;
    };
}
