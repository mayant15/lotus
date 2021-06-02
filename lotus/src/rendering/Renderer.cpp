#include "Renderer.h"

#include "RHI.h"
#include "passes/PBRPass.h"
#include "passes/SkyPass.h"

#include "debug/ShaderHotReload.h"

#include <lotus/Config.h>
#include <lotus/ecs/components/CCamera.h>


// NOTE: Initialize the viewport dims in editor/src/ui/panels.hpp with these values too just to be safe
constexpr float COLOR_BUFFER_HEIGHT = 720.0f;
constexpr float COLOR_BUFFER_WIDTH = 1280.0f;
constexpr float COLOR_BUFFER_ASPECT_RATIO = COLOR_BUFFER_WIDTH / COLOR_BUFFER_HEIGHT;

namespace Lotus::Renderer
{
    static Renderer::State state {};
    static std::vector<RenderPass*> passes {};

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

    void OnInit(const InitEvent& event)
    {
        // Setup options
        auto conf = GetRenderConfig();
        setFeatures(conf);

        // Setup debug context?
        if (GetBuildConfig().IsDebug)
        {
            RHI::EnableDebugContext();
            ShaderHotReloadInit();
        }

        // Setup a framebuffer for the viewport
        state.ViewportFBO = RHI::CreateFrameBuffer({true});
        state.ViewportColorAttachment = RHI::DefaultColorAttachment(state.ViewportFBO, COLOR_BUFFER_WIDTH, COLOR_BUFFER_HEIGHT);
        SetViewport(COLOR_BUFFER_WIDTH, COLOR_BUFFER_HEIGHT);
    }

    void SetViewport(unsigned int width, unsigned int height)
    {
        float ar  = (float) width / (float) height;

        if (width > COLOR_BUFFER_WIDTH || height > COLOR_BUFFER_HEIGHT)
        {
            if (ar >= COLOR_BUFFER_ASPECT_RATIO)
            {
                width = COLOR_BUFFER_WIDTH;
                height = COLOR_BUFFER_WIDTH / ar;
            }
            else
            {
                height = COLOR_BUFFER_HEIGHT;
                width = ar * COLOR_BUFFER_HEIGHT;
            }
        }

        state.AspectRatio = ar;
        state.ViewportHeight = height;
        state.ViewportWidth = width;
    }

    std::pair<float, float> GetViewportUV()
    {
        return { state.ViewportWidth / COLOR_BUFFER_WIDTH, state.ViewportHeight / COLOR_BUFFER_HEIGHT };
    }

    void OnSceneLoad(const SceneLoadEvent& event)
    {
        state.pEngineScene = event.pScene;
        // TODO: Shadow pass
//        passes.push_back(new ShadowPass(&state));
        passes.push_back(new PBRPass(&state));
        passes.push_back(new SkyPass(&state));
        state.isActive = true;
    }

    void OnPreUpdate(const PreUpdateEvent& event)
    {
        if (!state.isActive) return;

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
        if (!state.isActive) return;

        auto registry = state.pEngineScene->GetRegistry();
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

        // TODO: Fix viewport updates
        RHI::BindFrameBuffer(state.ViewportFBO);
        RHI::SetClearColor(0.24f, 0.24f, 0.24f, 1.0f);
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
        Entity entity = event.entity; // Take out the the const-ness
        auto&& [mr, tf] = entity.GetComponent<CMeshRenderer, CTransform>();

        ModelInfo info;
        info.id = (EntityID) entity;
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
