#include "Window.h"

#include <config.h>
#include <widgets.h>
#include <events.h>

#include <lotus/debug.h>
#include <lotus/ecs/EventManager.h>
#include <lotus/Input.h>

#include <imgui/imgui.h>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <string>

namespace Editor
{
    static void glfwErrorCallback(int error, const char* description)
    {
        LOG_ERROR("GLFW Error {}: {}", error, description);
    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        static std::unordered_map<unsigned int, unsigned int> buttonMap = {
                {GLFW_MOUSE_BUTTON_LEFT, Lotus::L_MOUSE_LEFT},
                {GLFW_MOUSE_BUTTON_RIGHT, Lotus::L_MOUSE_RIGHT},
                {GLFW_MOUSE_BUTTON_MIDDLE, Lotus::L_MOUSE_MIDDLE}
        };

        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse)
        {
            Lotus::Input::UpdateMouseButtonState(buttonMap.at(button), action != GLFW_RELEASE);

            // TODO: Layers
            auto& em = GET(Lotus::EventManager);
            em.Dispatch(Editor::MouseButtonEvent {});
        }
    }

    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse)
        {
            Lotus::Input::UpdateMouseState(xpos, ypos);

            // TODO: Layers
            auto& em = GET(Lotus::EventManager);
            em.Dispatch(Editor::MouseEvent {});
        }
    }

    Window* CreateNewWindow()
    {
        // Setup window
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit())
        {
            throw std::exception("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, OPENGL_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // Create window with graphics context
        GLFWwindow* window = glfwCreateWindow(1280, 720, "Lotus Editor", nullptr, nullptr);
        if (window == nullptr)
        {
            throw std::exception("Failed to create window");
        }
        glfwMakeContextCurrent(window);

        // Enable vsync
        glfwSwapInterval(1);

        // CallbacksI
        glfwSetCursorPosCallback(window, cursorPosCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);

        // Load glad
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw std::exception("Failed to initalize GLAD");
        }

        return window;
    }

    void StartFrame(Window* window)
    {
        glfwPollEvents();
    }

    void EndFrame(Window* window)
    {
        glfwSwapBuffers((GLFWwindow*) window);
    }

    bool ShouldCloseWindow(Window* window)
    {
        return glfwWindowShouldClose((GLFWwindow*) window);
    }

    void DestroyWindow(Window* window)
    {
        glfwDestroyWindow((GLFWwindow*) window);
        glfwTerminate();
    }
}
