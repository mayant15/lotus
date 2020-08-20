#pragma once

#include "lotus/lcommon.h"

namespace Lotus
{
    struct LOTUS_API Cubemap
    {
        unsigned int ID = 0;
        unsigned int VAO = 0;
        unsigned int VBO = 0;
    };

    LOADER(CubemapLoader, Cubemap)
    {
        SRef<Cubemap> Load(const std::vector<std::string>& faces) const;
    };
}
