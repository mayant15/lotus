#pragma once

#include <rendering/RenderPass.h>
#include <lotus/resources/HDRI.h>

namespace Lotus::Renderer
{
    class PBRPass final : public RenderPass
    {
        std::vector<PointLightInfo> pointLightParams;
        std::vector<SpotLightInfo> spotlightParams;
        std::vector<LightInfo> dirLightParams;
        const CTransform* pLightTransform;
        Matrix4f lightView;
        Matrix4f lightProjection;

        const Renderer::State* pState;
        const CSkybox* pSky;

    public:
        PBRPass(const Renderer::State* state) : pState(state)
        {
            //
        }

        void SetupFrame() override
        {
            auto registry = GetRegistry();

            // Process lighting
            pointLightParams.clear();
            auto ptView = registry->view<CPointLight, CTransform>();
            for (auto light : ptView)
            {
                const auto& [params, transform] = ptView.get<CPointLight, CTransform>(light);
                PointLightInfo info (params);
                info.position = transform.Position;
                pointLightParams.push_back(info);
            }

            spotlightParams.clear();
            auto spView = registry->view<CSpotlight, CTransform>();
            for (auto light : spView)
            {
                const auto& [params, transform] = spView.get<CSpotlight, CTransform>(light);
                SpotLightInfo info (params);
                info.position = transform.Position;
                info.direction = GetForwardVector(transform);
                spotlightParams.push_back(info);
            }

            dirLightParams.clear();
            auto dirView = registry->view<CLight, CTransform>();
            for (auto light : dirView)
            {
                const auto& [params, transform] = dirView.get<CLight, CTransform>(light);
                LightInfo info (params);
                info.direction = GetForwardVector(transform);
                dirLightParams.push_back(info);
            }
            pLightTransform = &dirView.get<CTransform>(dirView.front());

            auto skyView = registry->view<CSkybox>();
            if (!skyView.empty())
            {
                const auto& sky = skyView.get<CSkybox>(skyView.front());
                pSky = &sky;
            }
        }

        void RenderFrame() override
        {
            // Update parameters
            Vector3f forward = GetForwardVector(*pLightTransform);
            lightView = LLookAt(pLightTransform->Position, pLightTransform->Position + forward * 10.0f, UP);
            lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

            auto registry = GetRegistry();
            auto entityView = registry->view<CMeshRenderer, CTransform>();
            for (auto entity : entityView)
            {
                const auto&[data, transform] = entityView.get<CMeshRenderer, CTransform>(entity);
                drawMesh(data, transform);
            }
        }

    private:
        void drawMesh(const CMeshRenderer& data, const CTransform& transform)
        {
            Handle<Shader> shader = data.MeshMaterial->MaterialShader;
            shader->Use();

            // Set transforms and draw actor
            shader->SetMat4f("model", GetModelMatrix(transform));

            // Set camera
            shader->SetMat4f("view", pState->View);
            shader->SetMat4f("projection", pState->Projection);
            shader->SetMat4f("lightView", lightView);
            shader->SetMat4f("lightProjection", lightProjection);
            shader->SetVec3f("camPos", pState->CameraPos);

            // Set lighting
            shader->SetDirLightArray("dirLight", dirLightParams);
            shader->SetInt("numDirLight", dirLightParams.size());

            shader->SetPointLightArray("pointLight", pointLightParams);
            shader->SetInt("numPointLight", pointLightParams.size());

            shader->SetSpotlightArray("spotlight", spotlightParams);
            shader->SetInt("numSpotlight", spotlightParams.size());

            // Set material
            Handle<Material> material = data.MeshMaterial;
            shader->SetMaterial("material", material);

            // Setup the irradiance map for specular lighting
            RHI::SetActiveTextureSlot(1);
            RHI::BindCubeMap(pSky->Map->Irradiance);
            shader->SetInt("irradianceMap", 1);

            // TODO: Shadow pass
//            RHI::SetActiveTextureSlot(0);
//            RHI::BindTexture(shadowMap);
//            shader->SetInt("shadowMap", 0);

            std::vector<SubMesh> meshes = data.MeshModel->Meshes;
            for (SubMesh& mesh : meshes)
            {
                RHI::BindVAO(mesh.VAO);
                RHI::DrawElements(mesh.Indices.size(), nullptr);
            }
            RHI::BindVAO(0);
        }
    };
}
