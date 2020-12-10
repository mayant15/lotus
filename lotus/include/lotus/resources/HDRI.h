#pragma once

#include "lotus/lcommon.h"
#include "lotus/resources/Texture.h"
#include "lotus/resources/AssetRegistry.h"

namespace Lotus
{
    struct HDRI
    {
        unsigned int EnvironmentMap;
        unsigned int Irradiance;
    };

    LOADER(HDRI)
    {
        [[nodiscard]] SRef<HDRI> Load(const std::string& path) const;
    };

    struct CSkybox
    {
        Handle<HDRI> Map;
    };
}
