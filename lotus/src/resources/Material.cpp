#include "lotus/resources/Material.h"

namespace Lotus
{
    using json = nlohmann::json;

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
        // TODO: Load textures too
        // TODO: Create json helpers/serialization functions

        std::ifstream file (path);
        json data;
        file >> data;

        SRef<Material> material = std::make_shared<Material>();
        material->Diffuse = getVector(data, "diffuse");
        material->Specular = getVector(data, "specular");
        material->Shininess = data.at("shininess").get<float>();

        return material;
    }
}
