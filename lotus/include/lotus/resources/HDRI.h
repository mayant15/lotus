#pragma once

#include <lotus/filesystem.h>
#include <lotus/lcommon.h>
#include <lotus/resources/Texture.h>
#include <lotus/resources/AssetRegistry.h>
#include <lotus/ecs/ComponentRegistry.h>

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

        static std::string GetName()
        { return "CSkybox"; }
    };

    inline void to_json(nlohmann::json& data, const CSkybox& sb)
    {
        // TODO: Handle to path
    }

    inline void from_json(const nlohmann::json& data, CSkybox& sb)
    {
        sb.Map = LoadAsset<HDRI, HDRILoader>(ExpandPath(data.at("Map").get<std::string>()));
    }
}
