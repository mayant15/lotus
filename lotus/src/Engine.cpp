#include "lotus/Engine.h"
#include "rendering/opengl/GLRenderer.h"

namespace Lotus
{
    void Engine::Initialize(const LotusOp& options)
    {
        _renderer = &GLRenderer::Get();
        RendererOp rendererOp{
                .IsDebug = options.IsDebug,
                .RenderAPI = options.RenderAPI,
                .WindowOptions = {
                        .Width = options.Width,
                        .Height = options.Height
                }
        };
        _renderer->Initialize(rendererOp);
    }

    void Engine::Run()
    {
        tick();
    }

    void Engine::Shutdown()
    {
        _renderer->OnPreDestroy();
        _renderer->OnDestroy();
    }

    void Engine::tick()
    {
        double startTime = glfwGetTime();
        double currentTime = startTime;
        double lastTime = startTime;
        while (currentTime - startTime < 5)
        {
            currentTime = glfwGetTime();
            float delta = currentTime - lastTime;
            lastTime = currentTime;

            // Game logic tick
            // process physics, AI etc here

            // Render tick. Everything that needs to be done this frame should have been done by now.
            _renderer->OnPreUpdate();
            _renderer->OnUpdate(delta);
            _renderer->OnPostUpdate();

            // Poll for events
            // _eventSystem.Poll();
        }

        Shutdown();
    }
}
