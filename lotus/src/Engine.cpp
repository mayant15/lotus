#include "lotus/Engine.h"
#include "lotus/debug.h"
#include "GLRenderer.h"

namespace Lotus
{
    void Engine::OnEvent(Event& event)
    {
        Engine& engine = Engine::Get();
        if (event.Type == EEventType::WINDOW_CLOSE_EVENT)
        {
            engine._isRunning = false;
        }
        else
        {
            // Pass it to the event manager.
            // If it is an input event, invoke it immediately
            if (event.Type == EEventType::KEYBOARD_EVENT)
            {
                engine._eventManager->Invoke((KeyboardEvent&) event);
            }
            if (event.Type == EEventType::MOUSE_EVENT)
            {
                engine._eventManager->Invoke((MouseEvent&) event);
            }
            else
            {
                // TODO: cast to the appropriate type
//                engine._eventManager->Queue(event);
            }
        }
    }

    void Engine::Initialize(const LotusOp& options)
    {
        _eventManager = &EventManager::Get();

        _inputManager = &Input::Get();
        _eventManager->Bind<MouseEvent, &Input::UpdateMouseState>(_inputManager);
        _eventManager->Bind<KeyboardEvent, &Input::UpdateKeyState>(_inputManager);

        WindowOp winOptions{
                .Width = options.Width,
                .Height = options.Height,
                .IsDebug = options.IsDebug
        };

        // Create context
        switch (options.RenderAPI)
        {
            case ERenderAPI::OPEN_GL:
                _window = std::make_unique<GLWindow>(winOptions);
                break;
            case ERenderAPI::DIRECTX:
                LOG_ERROR("DirectX is not yet supported.");
                return;
            case ERenderAPI::VULKAN:
                LOG_ERROR("Vulkan is not yet supported.");
                return;
        }
        _window->SetEventCallback(Engine::OnEvent);

        switch (options.RenderAPI)
        {
            case ERenderAPI::OPEN_GL:
                _renderer = &GLRenderer::Get();
                break;
            case ERenderAPI::DIRECTX:
                LOG_ERROR("DirectX is not yet supported.");
                return;
            case ERenderAPI::VULKAN:
                LOG_ERROR("Vulkan is not yet supported.");
                return;
        }

        RendererOp rendererOp{
                .IsDebug = options.IsDebug,
                .RenderAPI = options.RenderAPI,
                .ViewportWidth = options.Width,
                .ViewportHeight = options.Height
        };
        _renderer->Initialize(rendererOp);
    }

    void Engine::Run()
    {
        double currentTime = glfwGetTime();
        double lastTime = currentTime;
        while (_isRunning)
        {
            // tick()
            currentTime = glfwGetTime();
            float delta = currentTime - lastTime;
            lastTime = currentTime;
            tick(delta);
        }

        Shutdown();
    }

    void Engine::Shutdown()
    {
        _renderer->OnPreDestroy();

        _renderer->OnDestroy();
        _window->OnDestroy();

        _renderer->OnShutdown();
        _window->OnShutdown();
    }

    void Engine::tick(float delta)
    {
        _eventManager->Queue(UpdateEvent{.DeltaTime = delta});
        // Game logic tick
        // TODO: process physics, AI etc here

        // Render tick. Everything that needs to be done this frame should have been done by now.
        _renderer->OnPreUpdate();
        _renderer->OnUpdate(delta);
        _renderer->OnPostUpdate();

        // Poll for events
        _window->OnPostUpdate();
        _eventManager->BroadcastQueue();
    }
}
