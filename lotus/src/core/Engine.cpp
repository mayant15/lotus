#include "Engine.h"

#include <lotus/ecs/EventManager.h>
#include <lotus/ecs/Entity.h>
#include <lotus/Input.h>
#include <lotus/Renderer.h>
#include <lotus/debug.h>

#include <core/platform/GLWindow.h>
#include <physics/Physics.h>
#include <rendering/opengl/GLRenderer.h>

namespace Lotus::Engine
{
    static Engine::State state {};
    static Renderer* renderer = nullptr;

    /**
     * @brief Event callback for propagating events triggered by the GLFW window
     * @param event Reference to the event
     */
    static void windowEventCallback(Event& event)
    {
        // TODO: Cast to appropriate event type
        auto& eventManager = GET(EventManager);
        if (event.Type == EEventType::KEYBOARD_EVENT)
        {
            eventManager.Dispatch((KeyboardEvent&) event);
        }
        else if (event.Type == EEventType::MOUSE_EVENT)
        {
            eventManager.Dispatch((MouseEvent&) event);
        }
        else if (event.Type == EEventType::WINDOW_CLOSE_EVENT)
        {
            // Don't really need to dispatch this event through the event system
            // because it can be dealt with here
            state.IsRunning = false;
        }
        else
        {}
    }

    /** @brief Create the main window and platform-specific context. Also register events. */
    void setupWindow()
    {
        // Create window
        auto renderConfig = GetRenderConfig();
        auto buildConfig = GetBuildConfig();

        WindowOp winOptions;
        winOptions.IsDebug = buildConfig.IsDebug;
        winOptions.Height = renderConfig.ViewportHeight;
        winOptions.Width = renderConfig.ViewportWidth;

        // Create context
        state.Window =  std::make_unique<GLWindow>(winOptions);
        state.Window->SetEventCallback(windowEventCallback);

        auto& eventManager = GET(EventManager);
        eventManager.Bind<PostUpdateEvent, &IWindow::OnPostUpdate>(state.Window.get());
        eventManager.Bind<DestroyEvent, &IWindow::OnDestroy>(state.Window.get());
        eventManager.Bind<ShutdownEvent, &IWindow::OnShutdown>(state.Window.get());
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
        auto renderConf = GetRenderConfig();
        switch (renderConf.RenderAPI)
        {
            case ERenderAPI::OPEN_GL: renderer = &GLRenderer::Get(); break;
            case ERenderAPI::DIRECTX: LOG_ERROR("DirectX is not yet supported."); break;
            case ERenderAPI::VULKAN: LOG_ERROR("Vulkan is not yet supported."); break;
        }
        renderer->Initialize(renderConf);

        auto& eventManager = GET(EventManager);
        eventManager.Bind<PreUpdateEvent, &Renderer::OnPreUpdate>(renderer);
        eventManager.Bind<UpdateEvent, &Renderer::OnUpdate>(renderer);
    }

    /** @brief Perform an engine tick and dispatch update events. */
    void tick(double delta)
    {
        auto& eventManager = GET(EventManager);

        eventManager.Dispatch(PreUpdateEvent {});

        UpdateEvent ue;
        ue.DeltaTime = delta;
        eventManager.Dispatch(ue);

        eventManager.Dispatch(PostUpdateEvent {});

        // Dispatch remaining queued events
        eventManager.DispatchAll();
    }

    void Initialize()
    {
        setupWindow();
        ECSInitialize();

        auto& eventManager = GET(EventManager);
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
        double currentTime = glfwGetTime();
        double lastTime = currentTime;
        while (state.IsRunning)
        {
            // tick
            currentTime = glfwGetTime();
            double delta = currentTime - lastTime;
            lastTime = currentTime;
            tick(delta);
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

        ECSShutdown();
    }
}
