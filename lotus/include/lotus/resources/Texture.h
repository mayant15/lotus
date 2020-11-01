#pragma once

#include "lotus/lcommon.h"

namespace Lotus
{
    struct Texture
    {
        unsigned int ID = 0;
    };

    LOADER(TextureLoader, Texture)
    {
        SRef<Texture> Load(const std::string& path) const;
    };
}
