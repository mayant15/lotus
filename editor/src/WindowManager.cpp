#include "WindowManager.h"

#include <lotus/debug.h>

#include <GLFW/glfw3.h>


namespace Lotus::Editor
{
    static void glfwErrorCallback(int error, const char *description)
    {
        LOG_ERROR("GLFW Error {}: {}", error, description);
    }

    WindowManager::WindowManager(Lotus::ERenderAPI backend)
    {
        // Setup window
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit())
        {
            throw std::runtime_error{"Failed to initialize GLFW"};
        }
    }

    auto WindowManager::CreateWindow -> Optional<Window>
    {
        CreateNewWindow()
    }
} // namespace Lotus::Editor
