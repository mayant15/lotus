#include "lotus/debug.h"
#include "GLWindow.h"

namespace Lotus
{
    static bool InitGLFW = true;

    static void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    static void closeWindowCallback(GLFWwindow* window){
        WindowCloseEvent event;
        auto function = (std::function<void(Event&)>*) glfwGetWindowUserPointer(window);
        (*function)(event);
    }

    GLWindow::GLWindow(const WindowOp& options) : Window(options)
    {
        if (InitGLFW)
        {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, options.IsDebug);
            InitGLFW = false;
        }

        _pWindow = glfwCreateWindow(options.Width, options.Height, options.Title.c_str(), nullptr, nullptr);
        if (_pWindow == nullptr)
        {
            LOG_ERROR("Failed to create window.");

            // TODO: Terminate this here or in the renderer? We may have multiple windows
            glfwTerminate();
            return;
        }

        glfwSetWindowUserPointer(_pWindow, &_dispatchEvent);

        glfwMakeContextCurrent(_pWindow);
        glfwSetFramebufferSizeCallback(_pWindow, framebufferSizeCallback);
        glfwSetWindowCloseCallback(_pWindow, closeWindowCallback);
    }

    void GLWindow::OnPostUpdate()
    {
        // Initialize and configure GLFW
        glfwSwapBuffers(_pWindow);
        glfwPollEvents();
    }

    void GLWindow::OnDestroy()
    {
        glfwDestroyWindow(_pWindow);
    }

    void GLWindow::OnShutdown()
    {
        glfwTerminate();
    }

    bool GLWindow::IsVSync() const
    {
        return false;
    }

    uint32_t GLWindow::GetWidth() const
    {
        return _options.Width;
    }

    uint32_t GLWindow::GetHeight() const
    {
        return _options.Height;
    }

    void* GLWindow::GetNativeWindow() const
    {
        return _pWindow;
    }

    void GLWindow::SetEventCallback(const std::function<void(Event&)>& callback)
    {
        _dispatchEvent = callback;
    }

    void GLWindow::SetVSync(bool enabled)
    {
        // TODO: Set VSync
    }
}
