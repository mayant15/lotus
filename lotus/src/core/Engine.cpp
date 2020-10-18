#include "Engine.h"

#include "lotus/debug.h"
#include "core/platform/GLWindow.h"

namespace Lotus
{
    /**
     * Receives window events and dispatches them appropriately
     * @param event Event to be dispatched
     */
    void windowEventCallback(Event& event)
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
            eventManager.Dispatch((WindowCloseEvent&) event);
        }
        else
        {}
    }

    void Engine::Initialize(const Config& config)
    {
        WindowOp winOptions { config };

        // Create context
        // TODO: Create somewhere else
        _window =  std::make_unique<GLWindow>(winOptions);
        _window->SetEventCallback(windowEventCallback);

        auto& eventManager = GET(EventManager);
        eventManager.Bind<PostUpdateEvent, &IWindow::OnPostUpdate>(_window.get());
        eventManager.Bind<DestroyEvent, &IWindow::OnDestroy>(_window.get());
        eventManager.Bind<ShutdownEvent, &IWindow::OnShutdown>(_window.get());

        eventManager.Bind<WindowCloseEvent, &Engine::OnWindowClose>(this);

        _systemRegistry = std::make_unique<SystemRegistry>(config);
        eventManager.Dispatch(InitEvent {});
    }

    void Engine::Run()
    {
        auto& eventManager = GET(EventManager);
        eventManager.Dispatch(BeginEvent {});

        // TODO: Use chrono?
        double currentTime = glfwGetTime();
        double lastTime = currentTime;
        while (_isRunning)
        {
            // tick()
            currentTime = glfwGetTime();
            double delta = currentTime - lastTime;
            lastTime = currentTime;
            tick(delta);
        }

        Shutdown();
    }

    void Engine::Shutdown()
    {
        LOG_INFO("Shutting down engine...");
        auto& eventManager = GET(EventManager);
        eventManager.Dispatch(PreDestroyEvent {});
        eventManager.Dispatch(DestroyEvent {});
        eventManager.Dispatch(ShutdownEvent {});

        // TODO: Shutdown system registry is required?
        _systemRegistry->Shutdown();
    }

    void Engine::tick(float delta)
    {
        UpdateEvent updateEvent;
        updateEvent.DeltaTime = delta;
        PreUpdateEvent preUpdateEvent;
        PostUpdateEvent postUpdateEvent;

        // Game logic tick
        // TODO: process physics, AI etc here

        auto& eventManager = GET(EventManager);

        eventManager.Dispatch(preUpdateEvent);
        eventManager.Dispatch(updateEvent);
        eventManager.Dispatch(postUpdateEvent);

        // Dispatch remaining queued events
        eventManager.DispatchAll();
    }

    void Engine::OnWindowClose(const WindowCloseEvent &event)
    {
        _isRunning = false;
    }
}
