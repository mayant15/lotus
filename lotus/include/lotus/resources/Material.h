#pragma once

#include "Texture.h"
#include "Shader.h"

#include <lotus/physics/Vector.h>

#include <variant>
#include <optional>

namespace Lotus
{
    struct Material
    {
        Handle<Shader> MaterialShader;
        std::variant<Vector3f, Handle<Texture>> Albedo;
        std::optional<Handle<Texture>> Normal;
        float Roughness = 1.0f;
        float Metallic = 0.0f;
        float AO = 1.0f;
    };

    LOADER(Material)
    {
        [[nodiscard]] SRef<Material> Load(const std::string& path) const;
    };
}
