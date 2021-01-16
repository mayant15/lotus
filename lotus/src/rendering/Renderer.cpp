#include "Renderer.h"

#include "RHI.h"
#include "passes/PBRPass.h"
#include "passes/SkyPass.h"

#include "debug/ShaderHotReload.h"

#include <lotus/Config.h>
#include <lotus/ecs/components/CCamera.h>

namespace Lotus::Renderer
{
    static Renderer::State state {};
    static std::vector<RenderPass*> passes {};

    void setState(const RenderConfig& conf)
    {
        state.ViewportHeight = conf.ViewportHeight;
        state.ViewportWidth = conf.ViewportWidth;
        state.AspectRatio = (double) state.ViewportWidth / state.ViewportHeight;
    }

    void setFeatures(const RenderConfig& conf)
    {
        unsigned int flags = 0;
        if (conf.DepthTest) flags |= RHI::ENABLE_DEPTH_TEST;
        if (conf.CullFace) flags |= RHI::ENABLE_CULL_FACE;
        if (conf.MSAA) flags |= RHI::ENABLE_MSAA;
        RHI::EnableFeatures(flags);

        // TODO: Alpha blending?
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void AddRenderPass(RenderPass* pass)
    {
        passes.push_back(pass);
    }

    void SetViewportWidth(int val)
    {
        state.ViewportWidth = val;
    }

    void SetViewportHeight(int val)
    {
        state.ViewportHeight = val;
    }

    void OnInit(const InitEvent& event)
    {
        // Setup options
        auto conf = GetRenderConfig();
        setState(conf);
        setFeatures(conf);

        // Setup debug context?
        if (GetBuildConfig().IsDebug)
        {
            RHI::EnableDebugContext();
            ShaderHotReloadInit();
        }

        RHI::SetClearColor(0.04f, 0.74f, 0.74f, 1.0f);

        // Setup a framebuffer for the viewport
        state.ViewportFBO = RHI::CreateFrameBuffer({true});
        state.ViewportColorAttachment = RHI::DefaultColorAttachment(state.ViewportFBO, 800, 600);
    }

    void OnBegin(const BeginEvent& event)
    {
        // TODO: Shadow pass
//        passes.push_back(new ShadowPass(&state));
        passes.push_back(new PBRPass(&state));
        passes.push_back(new SkyPass(&state));
    }

    void OnPreUpdate(const PreUpdateEvent& event)
    {
        auto registry = GetRegistry();
        auto cameraView = registry->view<CCamera, CTransform>();
        for (auto entity : cameraView)
        {
            const auto& [camera, transform] = cameraView.get<CCamera, CTransform>(entity);
            if (camera.IsActive)
            {
                state.CameraPos = transform.Position;
                state.View = GetViewMatrix(transform);
                state.Projection = LPerspective(glm::radians(camera.FOV), state.AspectRatio, 0.1f, 100.0f);

                // Set the results of the first active camera that you find, then break
                break;
            }
        }

        for (auto pass : passes)
        {
            pass->SetupFrame();
        }
    }

    unsigned int GetColorBuffer()
    {
        return state.ViewportColorAttachment;
    }

    void OnUpdate(const UpdateEvent& event)
    {
        // TODO: Fix viewport updates
        RHI::BindFrameBuffer(state.ViewportFBO);
        const auto a = state.ViewportHeight + state.ViewportWidth;
        RHI::SetClearColor(state.ViewportWidth / (float) a, state.ViewportHeight / (float) a, 1.0f, 1.0f);
        RHI::Clear(RHI::COLOR_BIT | RHI::DEPTH_BIT);
        RHI::SetViewport(state.ViewportWidth, state.ViewportHeight);

        // Call the update method for all render passes
        for (auto pass : passes)
        {
            pass->RenderFrame(event.DeltaTime);
        }
    }

    void OnDestroy(const DestroyEvent& event)
    {
        for (auto pass : passes)
        {
            delete pass;
        }
    }

    void OnMeshRendererCreate(const ComponentCreateEvent<CMeshRenderer>& event)
    {
        auto registry = GetRegistry();
        auto&& [mr, tf] = registry->get<CMeshRenderer, CTransform>(event.entityID);

        ModelInfo info;
        info.Transform = &tf;
        info.MeshMaterial = mr.MeshMaterial;

        for (const auto& mesh : mr.MeshModel->Meshes)
        {
            unsigned int vao, vbo, ebo;
            RHI::CreateBuffersForMesh(mesh.Vertices, mesh.Indices, &vao, &vbo, &ebo);
            info.Buffers.emplace_back(vbo, ebo, vao, mesh.Indices);
        }

        state.Queue.Insert(info);
    }
}
