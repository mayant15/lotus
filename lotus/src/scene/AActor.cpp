#include "lotus/scene/AActor.h"
#include "lotus/components.h"

namespace Lotus
{
    AActor::AActor(EntityID id, Scene* scene)
    {
        _id = id;
        _pScene = scene;
    }

    Vector3f AActor::GetPosition()
    {
        return GetTransform().position;
    }

    Vector3f AActor::GetRotation()
    {
        return GetTransform().rotation;
    }

    Vector3f AActor::GetForwardVector()
    {
        Vector3f rotation = GetRotation();
        float x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
        float y = sin(glm::radians(rotation.x));
        float z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
        return LNormalize(Vector3f(x, y, z));
    }

    Vector3f AActor::GetRightVector()
    {
        // TODO: A simple cross product doesn't account for rotation about the forward vector
        return LNormalize(LCross(GetForwardVector(), UP));
    }

    Vector3f AActor::GetUpVector()
    {
        return LNormalize(LCross(GetRightVector(), GetForwardVector()));
    }

    CTransform AActor::GetTransform()
    {
        return GetComponent<CTransform>();
    }
}
