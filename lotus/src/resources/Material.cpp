#include "lotus/resources/Material.h"

namespace Lotus
{
    SRef<Material> MaterialLoader::Load(std::variant<Vector3f, Handle<Texture>> diffuse, std::variant<Vector3f, Handle<Texture>> specular, float shininess) const
    {
        // TODO: Do the actual loading here too.

        SRef<Material> material = std::make_shared<Material>();
        material->Diffuse = diffuse;
        material->Specular = specular;
        material->Shininess = shininess;

        return material;
    }
}
