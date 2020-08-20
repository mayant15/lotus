#pragma once

#include "lotus/lcommon.h"

#define DIFFUSE_TEXTURE "texture_diffuse"
#define SPECULAR_TEXTURE "texture_specular"

#define RED GL_RED
#define RGB GL_RGB
#define RGBA GL_RGBA

namespace Lotus
{
    struct LOTUS_API Texture
    {
        uint32_t ID = 0;
        std::string Type;
    };

    LOADER(TextureLoader, Texture)
    {
        SRef<Texture> Load(const std::string& path, const std::string& type) const;
    };
}
