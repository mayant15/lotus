#include "lotus/Engine.h"

#include <physics/Physics.h>
#include <rendering/Renderer.h>

#include <lotus/ecs/EventManager.h>
#include <lotus/Input.h>
#include <lotus/debug.h>

namespace Lotus::Engine
{
    /** @brief Setup the physics subsystem and register events */
    void setupPhysics()
    {
        auto& eventManager = GET(EventManager);
        eventManager.Bind<InitEvent, Physics::OnInit>();
        eventManager.Bind<BeginEvent, Physics::OnBegin>();
        eventManager.Bind<UpdateEvent, Physics::OnUpdate>();
        eventManager.Bind<PostUpdateEvent, Physics::OnPostUpdate>();
        eventManager.Bind<DestroyEvent, Physics::OnDestroy>();
        eventManager.Bind<ComponentCreateEvent<CRigidBody>, Physics::OnRigidBodyCreate>();
    }

    /** @brief Setup the platform-specific renderer and register events */
    void setupRenderer()
    {
        auto& eventManager = GET(EventManager);
        eventManager.Bind<InitEvent, Renderer::OnInit>();
        eventManager.Bind<BeginEvent, Renderer::OnBegin>();
        eventManager.Bind<PreUpdateEvent, Renderer::OnPreUpdate>();
        eventManager.Bind<UpdateEvent, Renderer::OnUpdate>();
        eventManager.Bind<DestroyEvent, Renderer::OnDestroy>();
        eventManager.Bind<ComponentCreateEvent<CMeshRenderer>, Renderer::OnMeshRendererCreate>();
    }

    void Initialize()
    {
        ECSInitialize();

        auto& eventManager = GET(EventManager);
        eventManager.Bind<MouseEvent, Input::UpdateMouseState>();
        eventManager.Bind<KeyboardEvent, Input::UpdateKeyState>();

        setupPhysics();
        setupRenderer();

        eventManager.Dispatch(InitEvent {});
    }

    /** @brief Perform an engine tick and dispatch update events. */
    void Tick(double deltaTime)
    {
        auto& eventManager = GET(EventManager);

        eventManager.Dispatch(PreUpdateEvent {});

        UpdateEvent ue;
        ue.DeltaTime = deltaTime;
        eventManager.Dispatch(ue);

        eventManager.Dispatch(PostUpdateEvent {});

        // Dispatch remaining queued events
        eventManager.DispatchAll();
    }

    void Shutdown()
    {
        LOG_INFO("Shutting down engine...");
        auto& eventManager = GET(EventManager);
        eventManager.Dispatch(PreDestroyEvent {});
        eventManager.Dispatch(DestroyEvent {});
        eventManager.Dispatch(ShutdownEvent {});

        ECSShutdown();
    }
}
