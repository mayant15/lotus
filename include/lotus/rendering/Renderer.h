#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer
{
protected:
    GLFWwindow* window = nullptr;

public:
    virtual GLFWwindow* getActiveWindow() = 0;
    virtual void shutdown() = 0;
};

class GLRenderer : public Renderer
{
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    static void
    debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                         const void* userParam);

public:
    GLRenderer();

    void setViewport(int x, int y, int width, int height);

    GLFWwindow* getActiveWindow() override;

    void shutdown() override;
};

class VkRenderer : public Renderer
{

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    static void
    debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                         const void* userParam);

public:
    VkRenderer();

    void setViewport(int x, int y, int width, int height);

    GLFWwindow* getActiveWindow() override;

    void shutdown() override;
};
