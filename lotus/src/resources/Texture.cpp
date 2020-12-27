#include "lotus/resources/Texture.h"
#include "lotus/debug.h"

#include "rendering/RHI.h"

#include <stb_image.h>

namespace Lotus
{
    SRef<Texture> TextureLoader::Load(const std::string& path) const
    {
        SRef<Texture> texture = std::make_shared<Texture>();

        int width, height, nChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
        if (data)
        {
            RHI::TextureInfo info {};
            info.Data = data;
            info.Width = width;
            info.Height = height;
            info.Format = RHI::FormatFromChannel(nChannels);
            info.InternalFormat = info.Format; // Use the same format
            info.DataType = RHI::ETextureDataType::UNSIGNED_BYTE;
            texture->ID = RHI::CreateTexture(info);
        }
        else
        {
            LOG_ERROR("Texture failed to load at path: {}", path);
            stbi_image_free(data);
            throw std::invalid_argument("image data invalid");
        }

        return texture;
    }
}
