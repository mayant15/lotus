#include <lotus/resources/Material.h>
#include <lotus/filesystem.h>

#include <stb_image.h>

namespace Lotus
{
    using json = nlohmann::json;

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

    SRef<Material> MaterialLoader::Load(const std::string& path) const
    {
        std::ifstream file (path);
        json data;
        file >> data;

        SRef<Material> material = std::make_shared<Material>();

        // Albedo texture or color
        if (auto tex = getTexture(data, "Albedo"))
        {
            material->Albedo = tex.value();
        }
        else
        {
            material->Albedo = data["Albedo"].get<Vector3f>();
        }

        auto shader = data.at("Shader");
        material->MaterialShader = LoadAsset<Shader, ShaderLoader>(
            ExpandPath(shader.at("Vertex").get<std::string>()),
            ExpandPath(shader.at("Fragment").get<std::string>())
        );
        
        material->Roughness = data["Roughness"];
        material->AO = data["AO"];
        material->Metallic = data["Metallic"];
        material->Normal = getTexture(data, "Normal");

        return material;
    }
}
