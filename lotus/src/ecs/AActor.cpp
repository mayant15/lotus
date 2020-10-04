#include "lotus/ecs/AActor.h"

namespace Lotus
{
    Vector3f GetForwardVector(Entity actor) {
        auto& transform = actor.GetComponent<CTransform>();
        Vector3f rotation = transform.Rotation;
        float x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
        float y = sin(glm::radians(rotation.x));
        float z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
        return LNormalize(Vector3f(x, y, z));
    }

    Vector3f GetRightVector(Entity actor) {
        return LNormalize(LCross(GetForwardVector(actor), UP));
    }

    Vector3f GetUpVector(Entity actor) {
        return LNormalize(LCross(GetRightVector(actor), GetForwardVector(actor)));
    }
}
