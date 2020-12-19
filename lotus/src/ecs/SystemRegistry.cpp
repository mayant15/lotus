#include "SystemRegistry.h"

#include "lotus/debug.h"
#include "lotus/ecs/EventManager.h"

#include "rendering/opengl/GLRenderer.h"


namespace Lotus
{
    SystemRegistry::SystemRegistry()
    {
        // Create the entity registry
        ECSInitialize();

        auto& eventManager = GET(EventManager);

        // Create ECS
        // Register Events

        // Create Input and register events
        eventManager.Bind<MouseEvent, Input::UpdateMouseState>();
        eventManager.Bind<KeyboardEvent, Input::UpdateKeyState>();

        // Create Renderer
        auto renderConf = GetRenderConfig();
        switch (renderConf.RenderAPI)
        {
            case ERenderAPI::OPEN_GL: _renderer = &GLRenderer::Get(); break;
            case ERenderAPI::DIRECTX: LOG_ERROR("DirectX is not yet supported."); break;
            case ERenderAPI::VULKAN: LOG_ERROR("Vulkan is not yet supported."); break;
        }
        _renderer->Initialize(renderConf);

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
            eventManager.Bind<PostUpdateEvent, &PhysicsSubsystem::OnPostUpdate>(_physics.get());
            eventManager.Bind<DestroyEvent, &PhysicsSubsystem::OnDestroy>(_physics.get());
            eventManager.Bind<ComponentCreateEvent<CRigidBody>, &PhysicsSubsystem::OnRigidbodyCreate>(_physics.get());
        }
    }

    void SystemRegistry::Shutdown() const
    {
        ECSShutdown();
    }
}
