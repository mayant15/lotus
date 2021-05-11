#include <lotus/resources/HDRI.h>

#include <lotus/physics/Vector.h>
#include <lotus/resources/Shader.h>

#include <lotus/debug.h>

#include <rendering/RHI.h>
#include <rendering/Primitives.h>

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
    SRef<HDRI> HDRILoader::Load(const std::string& relpath) const
    {
        SRef<HDRI> hdri = std::make_shared<HDRI>();
        hdri->detail.path = relpath;
        std::string fullpath = ExpandPath(relpath);

        RHI::TextureInfo texInfo {};
        texInfo.Data = nullptr;
        texInfo.Width = 512;
        texInfo.Height = 512;
        texInfo.InternalFormat = RHI::EStorageFormat::RGB16F;
        texInfo.Format = RHI::EStorageFormat::RGB;
        auto envMap = RHI::CreateCubeMap(texInfo);

        texInfo.Width = 32;
        texInfo.Height = 32;
        auto irradianceMap = RHI::CreateCubeMap(texInfo);

        auto vao = RHI::GetCubeVAO();

        RHI::FrameBufferInfo fbInfo {};
        auto frameBuffer = RHI::CreateFrameBuffer(fbInfo);

        RHI::RenderBufferAttachmentInfo rbInfo {};
        rbInfo.Type = RHI::EAttachmentType::DEPTH;
        rbInfo.InternalFomat = RHI::EStorageFormat::DEPTH_COMPONENT24;
        rbInfo.Width = 512;
        rbInfo.Height = 512;
        RHI::AttachRenderBuffer(frameBuffer, rbInfo);

        stbi_set_flip_vertically_on_load(true);
        int width, height, nChannels;
        float* data = stbi_loadf(fullpath.c_str(), &width, &height, &nChannels, 0);
        if (data)
        {
            RHI::TextureInfo info {};
            info.Data = data;
            info.Height = height;
            info.Width = width;
            info.Format = RHI::FormatFromChannel(nChannels);
            info.InternalFormat = RHI::EStorageFormat::RGB16F;
            info.DataType = RHI::ETextureDataType::FLOAT;
            auto flatmap = RHI::CreateTexture(info);

            // convert HDR equirectangular environment map to cubemap equivalent
            auto shader = LoadAsset<Shader, ShaderLoader>(
                    "int://shaders/loadHDRI.vert",
                    "int://shaders/loadHDRI.frag"
            );

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
                RHI::TextureAttachmentInfo txAttInfo {};
                txAttInfo.Type = RHI::EAttachmentType::COLOR;
                txAttInfo.TextureTarget = RHI::GetCubeMapPositiveX() + i;
                txAttInfo.ID = envMap;
                RHI::AttachTexture(frameBuffer, txAttInfo);

                RHI::Clear(RHI::COLOR_BIT | RHI::DEPTH_BIT);
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
            LOG_ERROR("HDRI failed to load at path: {}", relpath);
            stbi_image_free(data);
            throw std::invalid_argument("Cubemap load failed.");
        }

        hdri->EnvironmentMap = envMap;

        // Create the irradiance map
        rbInfo.Height = 32;
        rbInfo.Width = 32;
        RHI::AttachRenderBuffer(frameBuffer, rbInfo);

        // convert HDR equirectangular environment map to cubemap equivalent
        auto convShader = LoadAsset<Shader, ShaderLoader>(
                "int://shaders/convolution.vert",
                "int://shaders/convolution.frag"
        );

        convShader->Use();
        convShader->SetInt("envMap", 1);
        convShader->SetMat4f("projection", captureProjection);

        RHI::SetActiveTextureSlot(1);
        RHI::BindCubeMap(envMap);

        // Configure the viewport to the capture dimensions
        RHI::SetViewport(32, 32); // The irradiance map is going to be 32x32
        RHI::BindFrameBuffer(frameBuffer);
        for (unsigned int i = 0; i < 6; ++i)
        {
            convShader->SetMat4f("view", captureViews[i]);

            // Attach color target and render
            RHI::TextureAttachmentInfo txAttInfo {};
            txAttInfo.Type = RHI::EAttachmentType::COLOR;
            txAttInfo.TextureTarget = RHI::GetCubeMapPositiveX() + i;
            txAttInfo.ID = irradianceMap;
            RHI::AttachTexture(frameBuffer, txAttInfo);

            RHI::Clear(RHI::COLOR_BIT | RHI::DEPTH_BIT);
            RHI::BindVAO(vao);
            RHI::DrawTriangles(36);
            RHI::BindVAO(0);
        }

        RHI::BindTexture(0);
        RHI::BindFrameBuffer(0);

        hdri->Irradiance = irradianceMap;

        return hdri;
    }
}
