#pragma once

#include "lotus/internal/entt.hpp"
#include "lotus/physics.h"

#define NULL_ENTITY entt::null

namespace Lotus
{
    class Entity;
    class AActor;
    class ACamera;

    typedef entt::registry EntityRegistry;
    typedef entt::entity EntityID;

    class Scene
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

        ACamera CreateCamera(const Vector3f& position, bool isActive);
        ACamera GetActiveCamera();
    };

    class Entity
    {
    protected:
        EntityID _id;
        // A non-owning reference. Memory is not an issue because the object that this points to
        // is handled by the SceneManage (unique_ptr).
        Scene* _pScene;
    public:
        Entity() : _id(NULL_ENTITY), _pScene(nullptr) {}
        Entity(EntityID id) : _id(id), _pScene(nullptr) {}
        Entity(EntityID id, Scene* scene) : _id(id), _pScene(scene) {}

        template<typename T, typename ...Args>
        T& AddComponent(Args&& ...args)
        {
            return _pScene->_registry.emplace<T>(_id, std::forward<Args>(args)...);
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
