#include "lotus/Engine.h"
#include "lotus/debug.h"
#include "GLRenderer.h"

#include <fstream>
#include "lotus/internal/cereal/cereal.hpp"
#include "lotus/scene/SceneManager.h"

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
        std::ofstream file("cache.json");
        cereal::JSONOutputArchive ar(file);

        const URef<Scene>& scene = SceneManager::Get().GetActiveScene();
        scene->Save(ar);

        _renderer->OnPreDestroy();

        _renderer->OnDestroy();
        _window->OnDestroy();

        // _renderer->OnShutdown();
        _window->OnShutdown();
    }

    void Engine::tick(float delta)
    {
        UpdateEvent updateEvent;
        updateEvent.DeltaTime = delta;
        _eventManager->Queue(updateEvent);
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
