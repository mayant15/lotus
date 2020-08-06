#include "lotus/scene/AActor.h"

namespace Lotus
{
    CTransform& AActor::GetTransform()
    {
        return GetComponent<CTransform>();
    }

    Vector3f AActor::GetAbsolutePosition()
    {
        return GetComponent<CTransform>().Position;
    }

    Vector3f AActor::GetAbsoluteRotation()
    {
        return GetComponent<CTransform>().Rotation;
    }

    Vector3f AActor::GetAbsoluteScale()
    {
        return GetComponent<CTransform>().Scale;
    }


    Vector3f AActor::GetForwardVector()
    {
        Vector3f rotation = GetAbsoluteRotation();
        float x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
        float y = sin(glm::radians(rotation.x));
        float z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
        return LNormalize(Vector3f(x, y, z));
    }

    Vector3f AActor::GetRightVector()
    {
        return LNormalize(LCross(GetForwardVector(), UP));
    }

    Vector3f AActor::GetUpVector()
    {
        return LNormalize(LCross(GetRightVector(), GetForwardVector()));
    }
}
