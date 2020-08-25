#pragma once

#include "lotus/lcommon.h"
#include "Texture.h"
#include "Shader.h"

#include <assimp/material.h>

#include <variant>

namespace Lotus
{
    struct MaterialDefaults
    {
        inline static Vector3f Specular {0.1f, 0.1f, 0.1f};
        inline static Vector3f Diffuse {0.5f, 0.5f, 0.5f};

        static Vector3f Get(const aiTextureType type)
        {
            if (type == aiTextureType_DIFFUSE) return Diffuse;
            else if (type == aiTextureType_SPECULAR) return Specular;
            else throw std::invalid_argument("Invalid texture type");
        }
    };

    struct Material
    {
        std::variant<Vector3f, Handle<Texture>> Diffuse;
        std::variant<Vector3f, Handle<Texture>> Specular;
        float Shininess = 1.0f;
    };

    LOADER(MaterialLoader, Material)
    {
        SRef<Material> Load(std::variant<Vector3f, Handle<Texture>> diffuse, std::variant<Vector3f, Handle<Texture>> specular, float shininess) const;
    };
}
