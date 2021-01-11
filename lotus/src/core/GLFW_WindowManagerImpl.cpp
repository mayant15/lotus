#include "WindowManager.h"

#include <GLFW/glfw3.h>

#include <lotus/ecs/EventManager.h>
#include <lotus/Input.h>
#include <lotus/Config.h>

namespace Lotus::WindowManager
{
    /** @brief GLFW constants for these inputs are serial, so just mapping KEY_A is enough */
    static constexpr unsigned int OFFSET_KEY_A = L_KEY_A - GLFW_KEY_A;

    /** @brief GLFW serial constants. Order is RELEASE, PRESS, REPEAT */
    static constexpr unsigned int OFFSET_KEY_RELEASE = L_KEY_RELEASE - GLFW_RELEASE;

    static bool InitGLFW = false;
    static GLFWwindow* pWindow = nullptr;
    static EventManager* pEventManager = nullptr;

    static void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    static void closeWindowCallback(GLFWwindow* window)
    {
        pEventManager->Dispatch(WindowCloseEvent {});
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        KeyboardEvent event;
        event.KeyCode = key + OFFSET_KEY_A;
        event.State = action + OFFSET_KEY_RELEASE;
        pEventManager->Dispatch((KeyboardEvent&) event);
    }

    static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
    {
        MouseEvent event;
        event.MouseX = xPos;
        event.MouseY = yPos;
        pEventManager->Dispatch((MouseEvent&) event);
    }

    static WindowInfo getWindowInfoFromConfig()
    {
        auto renderConf = GetRenderConfig();
        auto buildConf = GetBuildConfig();

        WindowInfo info {};
        info.Height = renderConf.ViewportHeight;
        info.Width = renderConf.ViewportWidth;
        info.IsDebug = buildConf.IsDebug;
        info.Title = "Lotus Game";

        return info;
    }

    void CreateWindow()
    {
        WindowInfo info = getWindowInfoFromConfig();
        
        if (!InitGLFW)
        {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, info.IsDebug);

            // 4x MSAA
            glfwWindowHint(GLFW_SAMPLES, 4);
            InitGLFW = true;
        }

        pEventManager = &GET(EventManager);
        pWindow = glfwCreateWindow(info.Width, info.Height, info.Title.c_str(), nullptr, nullptr);
        if (pWindow == nullptr)
        {
            glfwTerminate();
            throw std::exception("Failed to create window.");
        }
        glfwMakeContextCurrent(pWindow);

        // Setup callbacks
        glfwSetFramebufferSizeCallback(pWindow, framebufferSizeCallback);
        glfwSetWindowCloseCallback(pWindow, closeWindowCallback);
        glfwSetKeyCallback(pWindow, keyCallback);
        glfwSetCursorPosCallback(pWindow, cursorPositionCallback);
        
        glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void StartFrame()
    {
        //
    }

    void EndFrame()
    {
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    void DestroyWindow()
    {
        glfwDestroyWindow(pWindow);
        glfwTerminate();
    }

    std::pair<int, int> GetDimensions()
    {
        int height, width;
        glfwGetWindowSize(pWindow, &width, &height);
        return {width, height};
    }

    Window* GetWindowPointer()
    {
        return pWindow;
    }
}
