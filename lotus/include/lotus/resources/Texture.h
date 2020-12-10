#pragma once

#include "lotus/lcommon.h"
#include "lotus/resources/AssetRegistry.h"

namespace Lotus
{
    struct Texture
    {
        unsigned int ID = 0;
    };

    LOADER(Texture)
    {
        [[nodiscard]] SRef<Texture> Load(const std::string& path) const;
    };
}
