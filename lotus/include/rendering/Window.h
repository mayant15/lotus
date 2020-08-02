#pragma once
#include <memory>
#include <GLFW/glfw3.h>
#include <lotus/config.h>

class Window
{
    GLFWwindow* pGLWindow;
public:
    Window(Lotus::EContext context, unsigned int width, unsigned int height, const std::string& title);

    GLFWwindow* getGLWindow();
};

typedef std::unique_ptr<Window> URefWindow;