#pragma once

#include "lotus/ISerializable.h"
#include "lotus/internal/entt/entt.hpp"
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

    // TODO: Extract entity manipulations from the scene class into an EntityRegistry class. Scene should just contain a tree.
    class LOTUS_API Scene : public ISerializable
    {
        EntityRegistry _registry;
        friend Entity;
    public:

        std::string Path = "";

        template<typename... Component>
        auto Find()
        {
            return _registry.view<Component...>();
        }

        Entity CreateEntity();

        AActor CreateActor(const Vector3f& position);

        ACamera CreateCamera(const Vector3f& position, float fov, bool isActive);

        ACamera GetActiveCamera();

        void Load(IArchive& archive) override
        {
            archive(1.0f);
        }

        void Save(OArchive& archive) const override
        {
            archive(FIELD("sample", 30.12f));
        }

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
