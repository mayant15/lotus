#include <lotus/debug.h>
#include "lotus/rendering/Renderer.h"

GLRenderer::GLRenderer()
{
    // Initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window and an OpenGL context
    window = glfwCreateWindow(800, 600, "Title", nullptr, nullptr);
    if (window == nullptr)
    {
        LOG_ERROR("Failed to create window.");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD");
        return;
    }

    // Enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugMessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // Enable depth testing with the z-buffer
    glEnable(GL_DEPTH_TEST);
}

void GLRenderer::setViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void GLRenderer::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow *GLRenderer::getActiveWindow()
{
    return window;
}

void GLRenderer::shutdown()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

