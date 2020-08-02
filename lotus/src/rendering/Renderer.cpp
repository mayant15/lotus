#include "lotus/rendering.h"
#include "rendering/GLRenderer.h"

namespace Lotus
{
    void Renderer::init(bool isDebug)
    {
        GLRenderer& renderer = GLRenderer::get();
        renderer.init(isDebug);
    }

    void Renderer::renderScene(const Lotus::Scene& scene)
    {
        GLRenderer& renderer = GLRenderer::get();
        renderer.renderScene(scene);
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
}
