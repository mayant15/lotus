#include "lotus/debug.h"
#include "lotus/resources.h"
#include "lotus/rendering.h"
#include "stb_image.h"

namespace Lotus::Resource
{
    Texture::Texture(const std::string& path_, const std::string& type_) {
        path = path_;
        type = type_;
    }

    int Texture::import() {
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
        Lotus::Rendering::GLRenderer& renderer = Lotus::Rendering::GLRenderer::get();
        unsigned int textureID = renderer.createTexture(data, width, height, format);

        id = textureID;

        AssetManager::registerAsset(path);
        return IMPORT_SUCCESS_CODE;
    }
}