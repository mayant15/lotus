#include "lotus/resources/Material.h"
#include "lotus/debug.h"
#include "stb_image.h"
#include "lotus/resources/AssetRegistry.h"

namespace Lotus
{
    using json = nlohmann::json;

    // TODO: Create json helpers/serialization functions
    std::optional<Handle<Texture>> getTexture(const json& data, const std::string& key, bool flipY = true)
    {
        try
        {
            stbi_set_flip_vertically_on_load(flipY);
            auto handle = GET(AssetRegistry).LoadTexture(RESOURCE(data.at(key)));
            return std::optional<Handle<Texture>>{handle};
        }
        catch (const std::exception& e)
        {
            LOG_WARN(e.what());
        }

        return {};
    }

    Vector3f getVector(const json& data, const std::string& key)
    {
        return Vector3f(
            data.at(key).at("x").get<float>(),
            data.at(key).at("y").get<float>(),
            data.at(key).at("z").get<float>()
        );
    }

    SRef<Material> MaterialLoader::Load(const std::string& path) const
    {
        std::ifstream file (path);
        json data;
        file >> data;

        SRef<Material> material = std::make_shared<Material>();

        if (auto tex = getTexture(data, "albedo"))
        {
            material->Albedo = tex.value();
        }
        else
        {
            material->Albedo = getVector(data, "albedo");
        }
        
        material->Roughness = data["roughness"];
        material->AO = data["ao"];
        material->Metallic = data["metallic"];
        material->Normal = getTexture(data, "normal");

        return material;
    }
}
