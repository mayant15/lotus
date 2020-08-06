#include "lotus/scene/ACamera.h"

namespace Lotus
{
    Matrix4f ACamera::GetViewMatrix()
    {
        Vector3f position = GetAbsolutePosition();
        Vector3f forward = GetForwardVector();
        Vector3f up = GetUpVector();
        return LLookAt(position, position + forward, up);
    }

    float ACamera::GetFieldOfView()
    {
        auto camera = GetComponent<CCamera>();
        return camera.FOV;
    }
}
