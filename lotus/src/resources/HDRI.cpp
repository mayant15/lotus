#include "lotus/resources/HDRI.h"
#include "lotus/physics/physics.h"
#include "lotus/resources/AssetRegistry.h"
#include "lotus/debug.h"

#include "rendering/RHI.h"
#include "rendering/Primitives.h"

#include <stb_image.h>

namespace Lotus
{
    Matrix4f captureProjection = LPerspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    Matrix4f captureViews[] =
    {
        LLookAt(ORIGIN,         X_AXIS, -1.0f * Y_AXIS),
        LLookAt(ORIGIN, -1.0f * X_AXIS, -1.0f * Y_AXIS),
        LLookAt(ORIGIN,         Y_AXIS,         Z_AXIS),
        LLookAt(ORIGIN, -1.0f * Y_AXIS, -1.0f * Z_AXIS),
        LLookAt(ORIGIN,         Z_AXIS, -1.0f * Y_AXIS),
        LLookAt(ORIGIN, -1.0f * Z_AXIS, -1.0f * Y_AXIS),
    };

    // Reference: https://learnopengl.com/PBR/IBL/Diffuse-irradiance
    SRef<HDRI> HDRILoader::Load(const std::string& path) const
    {
        SRef<HDRI> hdri = std::make_shared<HDRI>();

        RHI::TextureInfo texInfo {};
        texInfo.Data = nullptr;
        texInfo.Width = 512;
        texInfo.Height = 512;
        texInfo.InternalFormat = RHI::ETextureFormat::RGB16F;
        texInfo.Format = RHI::ETextureFormat::RGB;
        auto envMap = RHI::CreateCubeMap(texInfo);
        auto irradianceMap = RHI::CreateCubeMap(texInfo);

        auto vao = RHI::GetCubeVAO();

        RHI::FrameBufferInfo fbInfo {};
        auto frameBuffer = RHI::CreateFrameBuffer(fbInfo);

        stbi_set_flip_vertically_on_load(true);
        int width, height, nChannels;
        float* data = stbi_loadf(path.c_str(), &width, &height, &nChannels, 0);
        if (data)
        {
            RHI::TextureInfo info {};
            info.Data = data;
            info.Height = height;
            info.Width = width;
            info.Format = RHI::FormatFromChannel(nChannels);
            info.InternalFormat = RHI::ETextureFormat::RGB16F;
            info.DataType = GL_FLOAT;
            auto flatmap = RHI::CreateTexture(info);

            // convert HDR equirectangular environment map to cubemap equivalent
            auto shader = GET(AssetRegistry).LoadShader(INTERNAL_SHADERS("loadHDRI"));

            shader->Use();
            shader->SetInt("hdri", 0);
            shader->SetMat4f("projection", captureProjection);

            RHI::SetActiveTextureSlot(0);
            RHI::BindTexture(flatmap);

            // Configure the viewport to the capture dimensions
            RHI::SetViewport(512, 512); // The cubemap face is going to be 512x512
            RHI::BindFrameBuffer(frameBuffer);
            for (unsigned int i = 0; i < 6; ++i)
            {
                shader->SetMat4f("view", captureViews[i]);

                // Attach color target and render
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envMap, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                RHI::BindVAO(vao);
                RHI::DrawTriangles(36);
                RHI::BindVAO(0);
            }

            RHI::BindTexture(0);
            RHI::BindFrameBuffer(0);
            stbi_image_free(data);
        }
        else
        {
            LOG_ERROR("HDRI failed to load at path: {}", path);
            stbi_image_free(data);
            throw std::invalid_argument("Cubemap load failed.");
        }

        hdri->EnvironmentMap = envMap;
        hdri->Irradiance = irradianceMap;
        return hdri;
    }
}
