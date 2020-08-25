#include "lotus/debug.h"
#include "lotus/resources/Texture.h"
#include "GLRenderer.h"
#include "stb_image.h"

namespace Lotus
{
    SRef<Texture> TextureLoader::Load(const std::string& path) const
    {
        int width, height, nrComponents;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        unsigned int format;
        if (data)
        {
            if (nrComponents == 1)
            {
                format = RED;
            }
            else if (nrComponents == 3)
            {
                format = RGB;
            }
            else if (nrComponents == 4)
            {
                format = RGBA;
            }
            else
            {
                LOG_ERROR("Cannot load texture. Invalid image color format.");
                stbi_image_free(data);
                throw std::invalid_argument("image color format unknown");
            }
        }
        else
        {
            LOG_ERROR("Texture failed to load at path: {}", path);
            stbi_image_free(data);
            throw std::invalid_argument("image data invalid");
        }

        // TODO: Make API independent. Fire an event?
        GLRenderer& renderer = GLRenderer::Get();
        const uint32_t textureID = renderer.createTexture(data, width, height, format);

        SRef<Texture> texture = std::make_shared<Texture>();
        texture->ID = textureID;
        return texture;
    }
}
