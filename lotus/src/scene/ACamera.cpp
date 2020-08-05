#include "lotus/scene/ACamera.h"

namespace Lotus
{
    ACamera::ACamera(EntityID id, Scene* scene)
    {
        _id = id;
        _pScene = scene;
    }

    Matrix4f ACamera::GetViewMatrix()
    {
        Vector3f position = GetPosition();
        return LLookAt(position, position + GetForwardVector(), GetUpVector());
    }

    float ACamera::GetFieldOfView()
    {
        auto camera = GetComponent<CCamera>();
        return camera.FOV;
    }
}
