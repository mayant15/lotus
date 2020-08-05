#pragma once
#include <GLFW/glfw3.h>
#include "lotus/lcommon.h"

namespace Lotus
{
    class Window
    {
        GLFWwindow* pGLWindow;
    public:
        Window(ERenderAPI context, unsigned int width, unsigned int height, const std::string& title);

        GLFWwindow* getGLWindow();
    };
}
