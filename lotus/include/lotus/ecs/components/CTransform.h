#pragma once

#include "lotus/physics/Vector.h"
#include "lotus/ecs/ComponentRegistry.h"

namespace Lotus
{
    struct CTransform
    {
        Vector3f Position {0.0f,  0.0f,  0.0f};
        Vector3f Rotation {0.0f, -90.0f, 0.0f};
        Vector3f Scale    {1.0f,  1.0f,  1.0f};

        GENERATED_BODY(CTransform, Position, Rotation, Scale);
    };

    inline Vector3f GetForwardVector(const CTransform& transform)
    {
        Vector3f rotation = transform.Rotation;
        float x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
        float y = sin(glm::radians(rotation.x));
        float z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
        return LNormalize(Vector3f(x, y, z));
    }

    inline Vector3f GetRightVector(const CTransform& transform)
    {
        return LNormalize(LCross(GetForwardVector(transform), UP));
    }

    inline Vector3f GetUpVector(const CTransform& transform)
    {
        return LNormalize(LCross(GetRightVector(transform), GetForwardVector(transform)));
    }

    inline Matrix4f GetModelMatrix(const CTransform& transform)
    {
        Matrix4f model(1.0f);
        model = LTranslate(model, transform.Position);
        model = LRotate(model, transform.Rotation.x, X_AXIS);
        model = LRotate(model, transform.Rotation.y, Y_AXIS);
        model = LRotate(model, transform.Rotation.z, Z_AXIS);
        model = LScale(model, transform.Scale);
        return model;
    }
}
