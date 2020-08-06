#include <lotus/debug.h>
#include "GLWindow.h"

namespace Lotus
{
    GLWindow::GLWindow(const WindowOp& options) : Window(options)
    {
        _pWindow = glfwCreateWindow(options.Width, options.Height, options.Title.c_str(), nullptr, nullptr);
        if (_pWindow == nullptr)
        {
            LOG_ERROR("Failed to create window.");
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(_pWindow);
    }

    void GLWindow::OnUpdate(float delta)
    {
        // TODO
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
        // TODO: Set event callback
    }

    void GLWindow::SetVSync(bool enabled)
    {
        // TODO: Set VSync
    }
}
