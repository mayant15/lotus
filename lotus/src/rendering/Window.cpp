#include "lotus/debug.h"
#include "rendering/Window.h"

Window::Window(Lotus::EContext context, unsigned int width, unsigned int height, const std::string& title)
{
    GLFWwindow* window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window_ == nullptr)
    {
        LOG_ERROR("Failed to create window.");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window_);

    pGLWindow = window_;
}

GLFWwindow* Window::getGLWindow()
{
    return pGLWindow;
}
