#pragma once

#include "lotus/physics.h"
#include "Texture.h"

#include <variant>
#include <optional>

namespace Lotus
{
    struct Material
    {
        std::variant<Vector3f, Handle<Texture>> Albedo;
        std::optional<Handle<Texture>> Normal;
        float Roughness;
        float Metallic;
        float AO;
    };

    LOADER(MaterialLoader, Material)
    {
        SRef<Material> Load(const std::string& path) const;
    };
}
