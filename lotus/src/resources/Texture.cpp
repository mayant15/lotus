#include <lotus/resources/Texture.h>
#include <lotus/debug.h>
#include <lotus/filesystem.h>

#include <rendering/RHI.h>

#include <stb_image.h>

namespace Lotus
{
    SRef<Texture> TextureLoader::Load(const std::string& relpath) const
    {
        SRef<Texture> texture = std::make_shared<Texture>();
        texture->detail.path = relpath;

        int width, height, nChannels;
        std::string fullpath = ExpandPath(relpath);
        unsigned char* data = stbi_load(fullpath.c_str(), &width, &height, &nChannels, 0);
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
            LOG_ERROR("Texture failed to load at path: {}", relpath);
            stbi_image_free(data);
            throw std::invalid_argument("image data invalid");
        }

        return texture;
    }
}
