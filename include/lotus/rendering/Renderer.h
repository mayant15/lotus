#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "lotus/scene.h"

class Renderer
{
protected:
    GLFWwindow* window = nullptr;

public:
    virtual GLFWwindow* getActiveWindow() = 0;

    virtual void shutdown() = 0;

    virtual void renderScene(const Scene& scene) = 0;
};

class GLRenderer : public Renderer
{
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

public:
    GLRenderer();

    void setViewport(int x, int y, int width, int height);

    GLFWwindow* getActiveWindow() override;

    void renderScene(const Scene& scene) override;

    void shutdown() override;
};

class VkRenderer : public Renderer
{

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

public:
    VkRenderer();

    void setViewport(int x, int y, int width, int height);

    GLFWwindow* getActiveWindow() override;

    void shutdown() override;
};
