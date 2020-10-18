#include "SystemRegistry.h"

#include "lotus/debug.h"
#include "lotus/ecs/EventManager.h"

#include "rendering/opengl/GLRenderer.h"


namespace Lotus
{
    SystemRegistry::SystemRegistry(const Config& config)
    {
        // Create the entity registry
        ECSInitialize();

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

        RendererOp rendererOp {};
        rendererOp.IsDebug = config.IsDebug;
        rendererOp.RenderAPI = config.RenderAPI;
        rendererOp.ViewportWidth = config.Width;
        rendererOp.ViewportHeight = config.Height;
        _renderer->Initialize(rendererOp);

        // Register Events
        eventManager.Bind<PreUpdateEvent, &Renderer::OnPreUpdate>(_renderer);
        eventManager.Bind<UpdateEvent, &Renderer::OnUpdate>(_renderer);

        // Create Physics
        _physics = std::make_unique<Physics::PhysicsSubsystem>();

        // Register Events
        {
            using namespace Physics;
            eventManager.Bind<InitEvent, &PhysicsSubsystem::OnInit>(_physics.get());
            eventManager.Bind<BeginEvent, &PhysicsSubsystem::OnBegin>(_physics.get());
            eventManager.Bind<UpdateEvent, &PhysicsSubsystem::OnUpdate>(_physics.get());
            eventManager.Bind<DestroyEvent, &PhysicsSubsystem::OnDestroy>(_physics.get());
            eventManager.Bind<ComponentCreateEvent<CRigidBody>, &PhysicsSubsystem::OnRigidbodyCreate>(_physics.get());
        }
    }

    void SystemRegistry::Shutdown() const
    {
        ECSShutdown();
    }
}
