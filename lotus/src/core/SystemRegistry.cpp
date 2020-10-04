#include "SystemRegistry.h"

#include "lotus/debug.h"
#include "lotus/ecs/EventManager.h"
#include "lotus/ecs/EntityRegistry.h"
#include "rendering/opengl/GLRenderer.h"

namespace Lotus
{
    void SystemRegistry::Initialize(const Config& config)
    {
        // Create the entity registry
        GET(EntityRegistry);

        auto& eventManager = GET(EventManager);

        // Create ECS
        // Register Events

        // Create Input and register events
        _input = &GET(Input);
        eventManager.Bind<MouseEvent, &Input::UpdateMouseState>(_input);
        eventManager.Bind<KeyboardEvent, &Input::UpdateKeyState>(_input);

        // Create Renderer
        switch (config.RenderAPI)
        {
            case ERenderAPI::OPEN_GL: _renderer = &GLRenderer::Get(); break;
            case ERenderAPI::DIRECTX: LOG_ERROR("DirectX is not yet supported."); break;
            case ERenderAPI::VULKAN: LOG_ERROR("Vulkan is not yet supported."); break;
        }

        RendererOp rendererOp;
        rendererOp.IsDebug = config.IsDebug;
        rendererOp.RenderAPI = config.RenderAPI;
        rendererOp.ViewportWidth = config.Width;
        rendererOp.ViewportHeight = config.Height;
        _renderer->Initialize(rendererOp);

        // Register Events
        // TODO: Bind with reflection
        eventManager.Bind<PreUpdateEvent, &Renderer::OnPreUpdate>(_renderer);
        eventManager.Bind<UpdateEvent, &Renderer::OnUpdate>(_renderer);

        // Create Physics
        // Register Events
    }

    void SystemRegistry::Update(float delta) const
    {
        //
    }

    void SystemRegistry::Shutdown() const
    {
        //
    }

//    void SubsystemManager::Start() const
//    {
//        // TODO: Create scenes through the scene manager
//        _pPhysics->CreateScene(Physics::PhysicsSceneInfo()); // default scene info
//    }

//    void SubsystemManager::Update(float delta) const
//    {
//        _pPhysics->Update(delta);
//    }

//    void SubsystemManager::Shutdown() const
//    {
//        _pPhysics->Shutdown();
//    }
}
