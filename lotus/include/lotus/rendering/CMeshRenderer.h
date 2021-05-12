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

        CMeshRenderer()
        {
            MeshMaterial = LoadAsset<Material, MaterialLoader>("int://materials/default.json");
            MeshModel = LoadAsset<Model, ModelLoader>("int://mesh/cube.fbx");
        }

        static std::string GetName()
        { return "CMeshRenderer"; }
    };

    inline void to_json(nlohmann::json& data, const CMeshRenderer& mr)
    {
        data["Model"] = mr.MeshModel->detail.path;
        data["Material"] = mr.MeshMaterial->detail.path;
    }

    inline void from_json(const nlohmann::json& data, CMeshRenderer& mr)
    {
        mr.MeshModel = LoadAsset<Model, ModelLoader>(data.at("Model").get<std::string>());
        mr.MeshMaterial = LoadAsset<Material, MaterialLoader>(data.at("Material").get<std::string>());
    }
}
