#include "lotus/debug.h"
#include "GLWindow.h"

namespace Lotus
{
    static bool InitGLFW = true;

    static int getKeyCode(int key)
    {
        switch (key)
        {
            case GLFW_KEY_W: return L_KEY_W;
            case GLFW_KEY_S: return L_KEY_S;
            case GLFW_KEY_A: return L_KEY_A;
            case GLFW_KEY_D: return L_KEY_D;
            case GLFW_KEY_E: return L_KEY_E;
            case GLFW_KEY_Q: return L_KEY_Q;
            default: return L_KEY_NONE;
        }
    }

    static int getState(int action)
    {
        switch (action)
        {
            case GLFW_PRESS:
                return L_KEY_PRESS;
            case GLFW_REPEAT:
                return L_KEY_REPEAT;
            case GLFW_RELEASE:
                return L_KEY_RELEASE;
            default:
                return L_KEY_NONE;
        }
    }

    static void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    static void closeWindowCallback(GLFWwindow* window)
    {
        WindowCloseEvent event;
        auto function = (std::function<void(Event)>*) glfwGetWindowUserPointer(window);
        (*function)(event);
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        KeyboardEvent event;
        event.KeyCode = getKeyCode(key);
        event.State = getState(action);

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
        glfwMakeContextCurrent(_pWindow);

        // Setup callbacks
        glfwSetWindowUserPointer(_pWindow, &_dispatchEvent);
        glfwSetFramebufferSizeCallback(_pWindow, framebufferSizeCallback);
        glfwSetWindowCloseCallback(_pWindow, closeWindowCallback);
        glfwSetKeyCallback(_pWindow, keyCallback);
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
        glfwSetWindowUserPointer(_pWindow, &_dispatchEvent);
    }

    void GLWindow::SetVSync(bool enabled)
    {
        // TODO: Set VSync
    }
}
