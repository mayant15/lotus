#pragma once

#include "lotus/lcommon.h"
#include "lotus/resources/Texture.h"

namespace Lotus
{
    struct HDRI
    {
        unsigned int EnvironmentMap;
        unsigned int Irradiance;
    };

    LOADER(HDRILoader, HDRI)
    {
        SRef<HDRI> Load(const std::string& path) const;
    };

    struct CSkybox
    {
        Handle<HDRI> Map;
    };
}
