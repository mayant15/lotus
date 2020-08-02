#include "lotus/rendering.h"

namespace Lotus
{
    AModel::AModel(const Vector3f& position_, const SRefModel& model_, const SRefShader& shader_) : AActor(position_)
    {
        model.model = model_;
        model.shader = shader_;
    }
}
