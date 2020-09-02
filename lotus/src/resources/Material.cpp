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

        material->Diffuse = getTexture(data, "diffuse");
        material->Specular = getTexture(data, "specular");
        material->Shininess = data.at("shininess").get<float>();

        return material;
    }
}
