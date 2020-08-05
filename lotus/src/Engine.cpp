#include "lotus/Engine.h"
#include "GLRenderer.h"

namespace Lotus
{
    void Engine::Initialize(const LotusOp& options)
    {
        _renderer = &GLRenderer::Get();
        RendererOp rendererOp{
                .IsDebug = options.IsDebug,
                .RenderAPI = options.RenderAPI,
                .Width = options.Width,
                .Height = options.Height
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
//        pollEvents();
//        update();
//        render();
            _renderer->OnPreUpdate();
            _renderer->OnUpdate(delta);
            _renderer->OnPostUpdate();
        }

        Shutdown();
    }
}
