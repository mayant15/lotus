#include <lotus/resources/Material.h>
#include <lotus/filesystem.h>

#include <stb_image.h>

namespace Lotus
{
    using json = nlohmann::json;

    // TODO: Create json helpers/serialization functions
    std::optional<Handle<Texture>> getTexture(const json& data, const std::string& key, bool flipY = true)
    {
        if (data.contains(key) && data.at(key).is_string())
        {
            stbi_set_flip_vertically_on_load(flipY);
            auto handle = LoadAsset<Texture, TextureLoader>(ExpandPath(data.at(key).get<std::string>()));
            return std::optional<Handle<Texture>>{handle};
        }
        else
        {
            return {};
        }
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
