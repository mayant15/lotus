#include "lotus/Engine.h"
#include "lotus/debug.h"
#include "GLRenderer.h"
#include "lotus/scene/SceneManager.h"
#include "lotus/Config.h"

namespace Lotus
{
    void Engine::OnEvent(Event& event)
    {
        Engine& engine = GET(Engine);
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

    void Engine::Initialize()
    {
        auto& options = GET(Config);

        _eventManager = &GET(EventManager);
        _inputManager = &GET(Input);
        _eventManager->Bind<MouseEvent, &Input::UpdateMouseState>(_inputManager);
        _eventManager->Bind<KeyboardEvent, &Input::UpdateKeyState>(_inputManager);

        WindowOp winOptions;
        winOptions.Width = options.Width;
        winOptions.Height = options.Height;
        winOptions.IsDebug = options.IsDebug;

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

        // TODO: Bind with reflection
        _eventManager->Bind<PostUpdateEvent, &IWindow::OnPostUpdate>(_window.get());
        _eventManager->Bind<DestroyEvent, &IWindow::OnDestroy>(_window.get());
        _eventManager->Bind<ShutdownEvent, &IWindow::OnShutdown>(_window.get());

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

        RendererOp rendererOp;
        rendererOp.IsDebug = options.IsDebug;
        rendererOp.RenderAPI = options.RenderAPI;
        rendererOp.ViewportWidth = options.Width;
        rendererOp.ViewportHeight = options.Height;
        _renderer->Initialize(rendererOp);

        // Register lifecycle events
        _eventManager->Bind<PreUpdateEvent, &Renderer::OnPreUpdate>(_renderer);
        _eventManager->Bind<UpdateEvent, &Renderer::OnUpdate>(_renderer);


        // Save scene
        _eventManager->Bind<PreDestroyEvent, &SceneManager::OnPreDestroy>(&SceneManager::Get());
    }

    void Engine::Run()
    {
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
        _eventManager->Invoke(PreDestroyEvent {});
        _eventManager->Invoke(DestroyEvent {});
        _eventManager->Invoke(ShutdownEvent {});
    }

    void Engine::tick(float delta)
    {
        UpdateEvent updateEvent;
        updateEvent.DeltaTime = delta;
        PreUpdateEvent preUpdateEvent;
        PostUpdateEvent postUpdateEvent;

        // Game logic tick
        // TODO: process physics, AI etc here

        _eventManager->Invoke(preUpdateEvent);
        _eventManager->Invoke(updateEvent);
        _eventManager->Invoke(postUpdateEvent);
    }
}
