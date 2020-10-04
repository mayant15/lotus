#include "IWindow.h"

namespace Lotus
{
    WindowOp::WindowOp(const Config& config)
        : Width(config.Width),
          Height(config.Height),
          IsDebug(config.IsDebug)
    {
        if (config.RenderAPI == ERenderAPI::OPEN_GL || config.RenderAPI == ERenderAPI::VULKAN)
        {
            Backend = EWindowBackend::GLFW;
        }
        else
        {
            Backend = EWindowBackend::DIRECTX;
        }
    }
}
