#pragma once

#include <lotus/lotus.h>
#include "lotus/rendering/Renderer.h"

namespace Lotus
{
    class GLRenderer : public Renderer, public Singleton<GLRenderer>
    {
        URef<Window> _window;
        RendererOp _options{};
        // framebuffer object
        unsigned int FBO = 0;

        // Texture used as a color buffer
        unsigned int texColorBuffer = 0;

        // Render buffer object for depth and stencil buffers
        unsigned int RBO = 0;

        Matrix4f view{};
        Matrix4f projection{};
        Vector3f cameraPos{};

    private:
        friend class Singleton<GLRenderer>;
        GLRenderer() = default;

        static void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int width, int height);

        static void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                         const GLchar* message,
                                         const void* userParam);

    public:

        URef <Window>& GetActiveWindow() override;

        void Initialize(const RendererOp& options) override;

        void SetViewport(int x, int y, int width, int height) override;

        void DrawMesh(const CMeshRenderer& data, const CTransform& transform) override;

        void OnPreUpdate() override;

        void OnUpdate(float delta) override;

        void OnPostUpdate() override;

        void OnPreDestroy() override;

        void OnDestroy() override;

        unsigned int createTexture(unsigned char* data, int width, int height, unsigned int format);
    };

}