#include "lotus/rendering.h"
#include "rendering/GLRenderer.h"

namespace Lotus
{
    void Renderer::init(Lotus::EContext context_, bool isDebug)
    {
        context = context_;
        GLRenderer& renderer = GLRenderer::get();
        renderer.init(isDebug);
    }

    void Renderer::shutdown()
    {
        GLRenderer& renderer = GLRenderer::get();
        renderer.shutdown();
    }

    void Renderer::setViewport(int x, int y, int width, int height)
    {
        GLRenderer& renderer = GLRenderer::get();
        renderer.setViewport(x, y, width, height);
    }

    void Renderer::update()
    {
        GLRenderer& renderer = GLRenderer::get();
        renderer.update();
    }

    Renderer& Renderer::get()
    {
        static Renderer instance;
        return instance;
    }

    void Renderer::renderModel(const SRefModel& model, const SRefShader& shader)
    {
        GLRenderer& renderer = GLRenderer::get();
        renderer.renderModel(model, shader);
    }

    void Renderer::prepareFrame(const SRefACamera& camera)
    {
        GLRenderer& renderer = GLRenderer::get();
        renderer.prepareFrame(camera);
    }

    void Renderer::swapBuffer()
    {
        GLRenderer& renderer = GLRenderer::get();
        renderer.swapBuffer();
    }
}
