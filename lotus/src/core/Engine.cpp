#include "Engine.h"

#include "core/WindowManager.h"

#include <physics/Physics.h>
#include <rendering/Renderer.h>

#include <lotus/ecs/EventManager.h>
#include <lotus/Input.h>
#include <lotus/debug.h>

#include <chrono>
#include <lotus/Config.h>

namespace Lotus::Engine
{
    static Engine::State state {};

    void onWindowClose(const WindowCloseEvent& event)
    {
        state.IsRunning = false;
    }

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

    /** @brief Perform an engine tick and dispatch update events. */
    void tick(double delta)
    {
        WindowManager::StartFrame();

        auto& eventManager = GET(EventManager);

        eventManager.Dispatch(PreUpdateEvent {});

        UpdateEvent ue;
        ue.DeltaTime = delta;
        eventManager.Dispatch(ue);

        eventManager.Dispatch(PostUpdateEvent {});

        // Dispatch remaining queued events
        eventManager.DispatchAll();

        WindowManager::EndFrame();
    }

    void Initialize()
    {
        ECSInitialize();

        WindowManager::CreateWindow();

        auto& eventManager = GET(EventManager);
        eventManager.Bind<WindowCloseEvent, onWindowClose>();

        eventManager.Bind<MouseEvent, Input::UpdateMouseState>();
        eventManager.Bind<KeyboardEvent, Input::UpdateKeyState>();

        setupPhysics();
        setupRenderer();

        eventManager.Dispatch(InitEvent {});
    }

    void Run()
    {
        GET(EventManager).Dispatch(BeginEvent {});

        // TODO: Use chrono?
        auto currentTime = std::chrono::system_clock::now();
        auto lastTime = currentTime;
        while (state.IsRunning)
        {
            // tick
            currentTime = std::chrono::system_clock::now();
            std::chrono::duration<double> delta = currentTime - lastTime;
            lastTime = currentTime;
            tick(delta.count());
        }

        Shutdown();
    }

    void Shutdown()
    {
        LOG_INFO("Shutting down engine...");
        auto& eventManager = GET(EventManager);
        eventManager.Dispatch(PreDestroyEvent {});
        eventManager.Dispatch(DestroyEvent {});
        eventManager.Dispatch(ShutdownEvent {});

        WindowManager::DestroyWindow();
        ECSShutdown();
    }
}
