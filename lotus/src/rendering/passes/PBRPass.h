#pragma once

#include <lotus/rendering/RenderPass.h>
#include <lotus/resources/HDRI.h>

namespace Lotus::Renderer
{
    class PBRPass final : public RenderPass
    {
        std::vector<PointLightInfo> pointLightParams;
        std::vector<SpotLightInfo> spotlightParams;
        std::vector<SunLightInfo> dirLightParams;
//        const CTransform* pLightTransform;
//        Matrix4f lightView;
//        Matrix4f lightProjection;

        const Renderer::State* pState;
        const CSkybox* pSky = nullptr;

    public:
        explicit PBRPass(const Renderer::State* state) : pState(state) {}

        void SetupFrame() override
        {
            auto registry = pState->pEngineScene->GetRegistry();

            // Process lighting
            pointLightParams.clear();
            auto ptView = registry->view<CPointLight, CTransform>();
            for (auto light : ptView)
            {
                const auto& [params, transform] = ptView.get<CPointLight, CTransform>(light);
                PointLightInfo info {};
                info.color = params.color;
                info.quadratic = params.quadratic;
                info.constant = params.constant;
                info.linear = params.linear;
                info.position = transform.Position;
                pointLightParams.push_back(info);
            }

            spotlightParams.clear();
            auto spView = registry->view<CSpotlight, CTransform>();
            for (auto light : spView)
            {
                const auto& [params, transform] = spView.get<CSpotlight, CTransform>(light);
                SpotLightInfo info {};
                info.linear = params.linear;
                info.constant = params.constant;
                info.quadratic = params.quadratic;
                info.color = params.color;
                info.innerCutOff = params.innerCutOff;
                info.outerCutOff = params.outerCutOff;
                info.position = transform.Position;
                info.direction = GetForwardVector(transform);
                spotlightParams.push_back(info);
            }

            dirLightParams.clear();
            auto dirView = registry->view<CSunLight>();
            for (auto light : dirView)
            {
                const auto& params = dirView.get<CSunLight>(light);
                SunLightInfo info {};
                info.color = params.color;
                info.direction = params.direction;
                dirLightParams.push_back(info);
            }

            auto skyView = registry->view<CSkybox>();
            if (!skyView.empty())
            {
                const auto& sky = skyView.get<CSkybox>(skyView.front());
                pSky = &sky;
            }
        }

        void RenderFrame(double deltaTime) override
        {
            // Update parameters
//            Vector3f forward = GetForwardVector(*pLightTransform);
//            lightView = LLookAt(pLightTransform->Position, pLightTransform->Position + forward * 10.0f, UP);
//            lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

            for (const auto& model : pState->Queue)
            {
                drawMesh(model);
            }
        }

    private:
        void drawMesh(const ModelInfo& model)
        {
            Handle<Material> material = model.MeshMaterial;
            Handle<Shader> shader = material->MaterialShader;
            const CTransform& tf = pState->pEngineScene->GetRegistry()->get<CTransform>(model.id);
            shader->Use();

            // Set transforms and draw actor
            shader->SetMat4f("model", GetModelMatrix(tf));

            // Set camera
            shader->SetMat4f("view", pState->View);
            shader->SetMat4f("projection", pState->Projection);
//            shader->SetMat4f("lightView", lightView);
//            shader->SetMat4f("lightProjection", lightProjection);
            shader->SetVec3f("camPos", pState->CameraPos);

            // Set lighting
            shader->SetDirLightArray("dirLight", dirLightParams);
            shader->SetInt("numDirLight", dirLightParams.size());

            shader->SetPointLightArray("pointLight", pointLightParams);
            shader->SetInt("numPointLight", pointLightParams.size());

            shader->SetSpotlightArray("spotlight", spotlightParams);
            shader->SetInt("numSpotlight", spotlightParams.size());

            // Set material
            shader->SetMaterial("material", material);

            // Setup the irradiance map for indirect diffuse lighting
            if (pSky)
            {
                RHI::SetActiveTextureSlot(0);
                RHI::BindCubeMap(pSky->Map->Irradiance);
                shader->SetInt("irradianceMap", 0);
                shader->SetBool("bUseIrradianceMap", true);
            }
            else
            {
                shader->SetInt("irradianceMap", 0);
                shader->SetBool("bUseIrradianceMap", false);
            }

            // TODO: Shadow pass
//            RHI::SetActiveTextureSlot(0);
//            RHI::BindTexture(shadowMap);
//            shader->SetInt("shadowMap", 0);

            for (const auto& buffer : model.Buffers)
            {
                RHI::BindVAO(buffer.VAO);
                RHI::DrawElements(buffer.Indices.size(), nullptr);
            }
            RHI::BindVAO(0);
        }
    };
}
