#pragma once

#include "lotus/physics/physics.h"
#include "lotus/resources/Texture.h"

#include <variant>
#include <optional>

namespace Lotus
{
    struct Material
    {
        std::variant<Vector3f, Handle<Texture>> Albedo;
        std::optional<Handle<Texture>> Normal;
        float Roughness = 1.0f;
        float Metallic = 0.0f;
        float AO = 1.0f;
    };

    LOADER(MaterialLoader, Material)
    {
        SRef<Material> Load(const std::string& path) const;
    };
}
