#include "lotus/debug.h"
#include "lotus/rendering.h"

namespace Lotus::Rendering
{
    Window::Window(Lotus::Context context, unsigned int width, unsigned int height, const std::string& title) {
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
}