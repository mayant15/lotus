#pragma once

#include "lotus/internal/entt.hpp"
#include "lotus/physics.h"
#include "lotus/lotus_export.h"

#define NULL_ENTITY entt::null  // NOLINT(cppcoreguidelines-macro-usage)

namespace Lotus
{
    class Entity;

    class AActor;

    class ACamera;

    typedef entt::registry EntityRegistry;
    typedef entt::entity EntityID;

    class LOTUS_API Scene
    {
    private:
        EntityRegistry _registry;
        friend Entity;
    public:
        template<typename... Component>
        auto Find()
        {
            return _registry.view<Component...>();
        }

        Entity CreateEntity();

        AActor CreateActor(const Vector3f& position);
//        AActor CreateActor(const CTransform& transform);

        ACamera CreateCamera(const Vector3f& position, float fov, bool isActive);

        ACamera GetActiveCamera();
    };

    class LOTUS_API Entity
    {
    protected:
        EntityID _id;
        // A non-owning reference. Memory is not an issue because the object that this points to
        // is handled by the SceneManage (unique_ptr).
        Scene* _pScene;
    public:
        Entity(EntityID id = NULL_ENTITY, Scene* scene = nullptr) : _id(id), _pScene(scene)
        {}

        template<typename T, typename ...Args>
        T& AddComponent(Args&& ...args)
        {
            return _pScene->_registry.emplace<T>(_id, args...);
        }

        template<typename T>
        T& GetComponent()
        {
            return _pScene->_registry.get<T>(_id);
        }

        void Destroy()
        {
            _pScene->_registry.destroy(_id);
        }
    };
}
