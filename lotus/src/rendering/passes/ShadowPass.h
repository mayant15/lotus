#pragma once

#include <rendering/RenderPass.h>
#include <lotus/ecs/Entity.h>

namespace Lotus::Renderer
{
    /**
     * @brief Create and update shadow maps
     */
    class ShadowPass final : public RenderPass
    {
        // Shadow map
        unsigned int shadowWidth = 1024;
        unsigned int shadowHeight = 1024;
        unsigned int fbo = 0;
        unsigned int depthTexture = 0;
        Handle<Shader> shader;

        // Directional light
        const CLight* light;
        const CTransform* transform;

        // Pipeline
        const Renderer::State* pState;

    public:
        ShadowPass(const Renderer::State* state) : pState(state)
        {
            // Setup FBO for shadow maps
            RHI::FrameBufferInfo info {};
            info.HasColorBuffer = false;
            fbo = RHI::CreateFrameBuffer(info);

            RHI::TextureInfo textureInfo {};
            textureInfo.Width = shadowWidth;
            textureInfo.Height = shadowHeight;
            textureInfo.Data = nullptr;
            textureInfo.DataType = RHI::ETextureDataType::FLOAT;
            textureInfo.Format = RHI::EStorageFormat::DEPTH_COMPONENT;
            textureInfo.InternalFormat = RHI::EStorageFormat::DEPTH_COMPONENT;

            // TODO: Mipmap levels
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            RHI::CreateTexture(textureInfo);

//            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

            RHI::TextureAttachmentInfo attInfo {};
            attInfo.ID = depthTexture;
            attInfo.TextureTarget = RHI::GetTexture2DTarget();
            attInfo.Type = RHI::EAttachmentType::DEPTH;
            RHI::BindFrameBuffer(fbo);
            RHI::AttachTexture(fbo, attInfo);

            // Unbind the FBO now that everything has been set up
            RHI::BindFrameBuffer(0);

            // Setup internal shaders
            shader = LoadAsset<Shader, ShaderLoader>(
                    ExpandPath("int://shaders/shadow.vert"),
                    ExpandPath("int://shaders/shadow.frag")
            );

            // Find the first directional light
            auto registry = GetRegistry();
            auto dirView = registry->view<CLight, CTransform>();
            if (!dirView.empty())
            {
                const auto& [lt, tf] = dirView.get<CLight, CTransform>(dirView.front());
                light = &lt;
                transform = &tf;
            }
        }

        void RenderFrame() override
        {
            // Update parameters
            Vector3f forward = GetForwardVector(*transform);
            Matrix4f lightView = LLookAt(transform->Position, transform->Position + forward * 10.0f, UP);
            Matrix4f lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

            // TODO: Bake the shadow map, don't generate it on update
            // Configure renderer to render the shadow map
            RHI::SetViewport(shadowWidth, shadowHeight);
            RHI::BindFrameBuffer(fbo);
            RHI::Clear(RHI::DEPTH_BIT);
            RHI::SetCullFace(RHI::CULL_FACE_FRONT);

            // Setup shader uniforms
            shader->Use();
            shader->SetMat4f("lightView", lightView);
            shader->SetMat4f("lightProjection", lightProjection);

            // Render entities to shadow map
            auto registry = GetRegistry();
            auto entityView = registry->view<CMeshRenderer, CTransform>();
            for (auto entity : entityView)
            {
                const auto&[data, tf] = entityView.get<CMeshRenderer, CTransform>(entity);
                shader->SetMat4f("model", GetModelMatrix(tf));

                RHI::SetActiveTextureSlot(0);
                RHI::BindTexture(depthTexture);
                for (const auto& mesh : data.MeshModel->Meshes)
                {
                    RHI::BindVAO(mesh.VAO);
                    RHI::DrawElements(mesh.Indices.size(), nullptr);
                }

                RHI::BindVAO(0);
            }

            // Reset renderer options
            RHI::SetViewport(pState->ViewportWidth, pState->ViewportHeight);
            RHI::BindFrameBuffer(0);
            RHI::SetCullFace(RHI::CULL_FACE_BACK);
        }
    };
}
