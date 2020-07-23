#pragma once

#include <imgui.h>
#include <memory>
#include <GLFW/glfw3.h>
#include "lotus/lotus.h"
#include "UIRenderBackend.h"

#define GLFW_HAS_WINDOW_TOPMOST       (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ GLFW_FLOATING
#define GLFW_HAS_WINDOW_HOVERED       (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ GLFW_HOVERED
#define GLFW_HAS_WINDOW_ALPHA         (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwSetWindowOpacity
#define GLFW_HAS_PER_MONITOR_DPI      (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwGetMonitorContentScale
#define GLFW_HAS_VULKAN               (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ glfwCreateWindowSurface

class GUI
{
    GLFWwindow* window;
    std::unique_ptr<UIRenderBackend> backend;
    double time;
    bool mouseJustPressed[ImGuiMouseButton_COUNT] = {};
    GLFWcursor* mouseCursors[ImGuiMouseCursor_COUNT] = {};

    static const char* getClipboardText(void* userData);
    static void setClipboardText(void* userData, const char* text);

    void updateMouseCursor();
    void updateMousePosAndButtons();

public:
    GUI(GLFWwindow* window_, Context context_);
    void newFrame();

    // TODO: Input system should call these after the usual registered callbacks
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void charCallback(GLFWwindow* window, unsigned int c);

    void render();

    void shutdown();
};
