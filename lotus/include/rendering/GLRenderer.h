#pragma once

#include "IRenderer.h"
#include "lotus/rendering.h"

class GLRenderer : public IRenderer
{
    // Framebuffer object
    unsigned int FBO = 0;

    // Texture used as a color buffer
    unsigned int texColorBuffer = 0;

    // Render buffer object for depth and stencil buffers
    unsigned int RBO = 0;

    Matrix4f view;
    Matrix4f projection;
    Vector3f cameraPos;

    GLRenderer() = default;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    static void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                     const GLchar* message,
                                     const void* userParam);

public:

    GLRenderer(const GLRenderer&) = delete;

    GLRenderer& operator=(const GLRenderer&) = delete;

    static GLRenderer& get();

    void setViewport(int x, int y, int width, int height) const;

    URefWindow& getActiveWindow() override;

    void init(bool isDebug) override;

    void renderModel(const Lotus::SRefModel& model, const Lotus::SRefShader& shader);

    void shutdown() override;

    void update() override;

    unsigned int createTexture(unsigned char* data, int width, int height, unsigned int format);

    void prepareFrame(const Lotus::SRefACamera& camera);

    void swapBuffer();
};
