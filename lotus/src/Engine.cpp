#include "lotus/Engine.h"
#include "lotus/debug.h"
#include "GLRenderer.h"

namespace Lotus
{
    static void dispatchEvent(Event& event)
    {
        // Handle events relevant to the engine, broadcast the rest
        if (event.Type == EEventType::WINDOW_CLOSE_EVENT)
        {
            Engine::Get().Stop();
        }
    }

    void Engine::Initialize(const LotusOp& options)
    {
        WindowOp winOptions {
                .Width = options.Width,
                .Height = options.Height,
                .IsDebug = options.IsDebug
        };

        // Create context
        switch (options.RenderAPI)
        {
            case ERenderAPI::OPEN_GL:
                _window = std::make_unique<GLWindow>(winOptions); break;
            case ERenderAPI::DIRECTX:
                LOG_ERROR("DirectX is not yet supported."); return;
            case ERenderAPI::VULKAN:
                LOG_ERROR("Vulkan is not yet supported."); return;
        }

        _window->SetEventCallback(dispatchEvent);

        _renderer = &GLRenderer::Get();
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
        double startTime = glfwGetTime();
        double currentTime = startTime;
        double lastTime = startTime;
        while (_isRunning)
        {
            // tick()

            currentTime = glfwGetTime();
            float delta = currentTime - lastTime;
            lastTime = currentTime;

            // Game logic tick
            // TODO: process physics, AI etc here

            // Render tick. Everything that needs to be done this frame should have been done by now.
            _renderer->OnPreUpdate();
            _renderer->OnUpdate(delta);
            _renderer->OnPostUpdate();

            // Poll for events
            _window->OnPostUpdate();
        }

        Shutdown();
    }

    void Engine::Stop()
    {
        _isRunning = false;
    }

    void Engine::Shutdown()
    {
        _renderer->OnPreDestroy();

        _renderer->OnDestroy();
        _window->OnDestroy();

        _renderer->OnShutdown();
        _window->OnShutdown();
    }

    void Engine::tick()
    {

    }
}
