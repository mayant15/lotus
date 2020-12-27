#pragma once

#include <lotus/lcommon.h>
#include <lotus/filesystem.h>
#include <lotus/resources/Model.h>
#include <lotus/resources/Material.h>

namespace Lotus
{
    struct CMeshRenderer
    {
        Handle<Material> MeshMaterial;
        Handle<Model> MeshModel;
        // NOTE: These cannot be just "Material" or "Model" because GCC complains about the name
        // MSVC and clang will work fine though

        REGISTER_DECL();
    };

    inline void to_json(nlohmann::json& data, const CMeshRenderer& mr)
    {
        // TODO: Handle to path
    }

    inline void from_json(const nlohmann::json& data, CMeshRenderer& mr)
    {
        mr.MeshModel = LoadAsset<Model, ModelLoader>(ExpandPath(data.at("Model").get<std::string>()));
        mr.MeshMaterial = LoadAsset<Material, MaterialLoader>(ExpandPath(data.at("Material").get<std::string>()));
    }

    REGISTER_BODY(CMeshRenderer);
}
