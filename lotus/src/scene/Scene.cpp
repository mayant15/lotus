#include "lotus/components.h"
#include "lotus/scene/Scene.h"
#include "lotus/scene/ACamera.h"
#include "lotus/debug.h"

namespace Lotus
{
    Entity Scene::CreateEntity()
    {
        EntityID id = _registry.create();
        return Entity(id, this);
    }

    AActor Scene::CreateActor(const Vector3f& position)
    {
        EntityID id = _registry.create();
        CTransform& transform = _registry.emplace<CTransform>(id);
        transform.position = position;

        return AActor(id, this);
    }

    ACamera Scene::CreateCamera(const Vector3f& position, bool isActive)
    {
        CTransform transform {
            .position = position
        };

        CCamera camera {
            .IsActive = isActive,
            .FOV = 45
        };

        // TODO: This should happen through prefabs
        EntityID id = _registry.create();
        _registry.emplace<CTransform>(id, transform);
        _registry.emplace<CCamera>(id, camera);

        return ACamera(id, this);
    }

    ACamera Scene::GetActiveCamera()
    {
        auto view = Find<CCamera>();
        for (const EntityID& id : view)
        {
            CCamera& camera = view.get(id);
            if (camera.IsActive)
            {
                return ACamera(id, this);
            }
        }

        LOG_ERROR("No active camera in scene.");

        // TODO: Custom exceptions
        throw std::exception();
    }
}