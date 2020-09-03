#include "lotus/resources/Material.h"

#include "stb_image.h"
#include "lotus/resources/AssetRegistry.h"

namespace Lotus
{
    using json = nlohmann::json;

    // TODO: Create json helpers/serialization functions
    std::variant<Vector3f, Handle<Texture>> getTexture(const json& data, const std::string& key, bool flipY = true)
    {
        if (data.at(key).is_string())
        {
            stbi_set_flip_vertically_on_load(flipY);
            auto& cache = AssetRegistry::Get();
            return cache.LoadTexture(data.at(key));
        }
        else
        {
            return Vector3f( 
                data.at(key).at("x").get<float>(),
                data.at(key).at("y").get<float>(),
                data.at(key).at("z").get<float>()
            );
        }
    }

    SRef<Material> MaterialLoader::Load(const std::string& path) const
    {

        std::ifstream file (path);
        json data;
        file >> data;

        SRef<Material> material = std::make_shared<Material>();

        json albedo = data["albedo"];
        // material->Albedo = Vector3f(albedo["x"].get<float>(), albedo["y"].get<float>(), albedo["z"].get<float>());
        material->Albedo = Vector3f(albedo["x"], albedo["y"], albedo["z"]);
        material->Roughness = data["roughness"];
        material->AO = data["ao"];
        material->Metallic = data["metallic"];

        return material;
    }
}
