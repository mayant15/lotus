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
            auto handle = LoadAsset<Texture, TextureLoader>(data.at(key).get<std::string>());
            return std::optional<Handle<Texture>> {handle};
        }
        else
        {
            return {};
        }
    }

    SRef<Material> MaterialLoader::Load(const std::string& relpath) const
    {
        std::ifstream file(ExpandPath(relpath));
        json data;
        file >> data;

        SRef<Material> material = std::make_shared<Material>();
        material->detail.path = relpath;

        // Albedo texture or color
        if (auto tex = getTexture(data, "Albedo"))
        {
            material->Albedo = tex.value();
        }
        else
        {
            material->Albedo = data["Albedo"].get<Color>();
        }

        // AO texture or color
        if (auto tex = getTexture(data, "AO"))
        {
            material->AO = tex.value();
        }
        else
        {
            material->AO = data["AO"].get<float>();
        }

        auto shader = data.at("Shader");
        material->MaterialShader = LoadAsset<Shader, ShaderLoader>(
                shader.at("Vertex").get<std::string>(),
                shader.at("Fragment").get<std::string>()
        );

        material->Roughness = data["Roughness"];
        material->Metallic = data["Metallic"];
        material->Normal = getTexture(data, "Normal");

        return material;
    }
}
