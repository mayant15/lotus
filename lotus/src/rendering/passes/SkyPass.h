#pragma once

#include <rendering/RenderPass.h>

#include <lotus/ecs/Entity.h>
#include <lotus/resources/HDRI.h>
#include <rendering/Primitives.h>

namespace Lotus::Renderer
{
    /** @brief Render the skybox, executed at the end of the pipeline */
    class SkyPass final : public RenderPass
    {
        Handle<Shader> shader;
        const CSkybox* pSky = nullptr;
        const Renderer::State* pState = nullptr;

        bool isActive = false;

    public:
        explicit SkyPass(const Renderer::State* state) : pState(state)
        {
            auto registry = GetRegistry();
            auto skyView = registry->view<CSkybox>();
            if (!skyView.empty())
            {
                // Get the first skybox
                // NOTE: Use a pointer here so that changes to the skybox during runtime are always reflected
                pSky = &skyView.get<CSkybox>(skyView.front());
                isActive = true;
            }

            shader = LoadAsset<Shader, ShaderLoader>(
                    ExpandPath("int://shaders/skybox.vert"),
                    ExpandPath("int://shaders/skybox.frag")
            );
        }

        void RenderFrame(double deltaTime) override
        {
            if (!isActive)
            {
                return;
            }

            // Set the depth function so that the sky is rendered at the background only
            RHI::SetDepthFunction(RHI::DEPTH_FUNC_LEQUAL);

            // Setup shader uniforms
            shader->Use();
            shader->SetMat4f("view", Matrix4f (Matrix3f (pState->View)));
            shader->SetMat4f("projection", pState->Projection);

            // Render to cube
            RHI::BindVAO(RHI::GetCubeVAO());
            RHI::SetActiveTextureSlot(0);
            shader->SetInt("skybox", 0);
            RHI::BindCubeMap(pSky->Map->EnvironmentMap);
            RHI::DrawTriangles(36);
            RHI::BindVAO(0);

            // Reset depth function
            RHI::SetDepthFunction(RHI::DEPTH_FUNC_LESS);
        }
    };
}
