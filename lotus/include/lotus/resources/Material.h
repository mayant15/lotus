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
        std::variant<Color, Handle<Texture>> Albedo;
        std::variant<float, Handle<Texture>> AO;
        std::optional<Handle<Texture>> Normal;
        float Roughness = 1.0f;
        float Metallic = 0.0f;

        struct {
            std::string path;
        } detail;
    };

    LOADER(Material)
    {
        [[nodiscard]] SRef<Material> Load(const std::string& relpath) const;
    };
}
