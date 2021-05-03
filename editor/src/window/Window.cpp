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
#include <stdexcept>

namespace Editor
{
    static constexpr unsigned int OFFSET_KEY_A = Lotus::L_KEY_A - GLFW_KEY_A;
    static constexpr unsigned int OFFSET_KEY_RELEASE = Lotus::L_KEY_RELEASE - GLFW_RELEASE;

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

    static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Lotus::Input::UpdateKeyState(key + OFFSET_KEY_A, action + OFFSET_KEY_RELEASE);
        auto& em = Lotus::EventManager::Get();
        em.Dispatch(Editor::KeyboardEvent {});

        // TODO: Call this only if not on a widget
        em.Dispatch(Lotus::KeyboardEvent {});
    }

    Window* CreateNewWindow()
    {
        // Setup window
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit())
        {
            throw std::runtime_error { "Failed to initialize GLFW" };
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, OPENGL_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // Create window with graphics context
        GLFWwindow* window = glfwCreateWindow(1280, 720, "Lotus Editor", nullptr, nullptr);
        if (window == nullptr)
        {
            throw std::runtime_error { "Failed to create window" };
        }
        glfwMakeContextCurrent(window);

        // Enable vsync
        glfwSwapInterval(1);

        // CallbacksI
        glfwSetCursorPosCallback(window, cursorPosCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetKeyCallback(window, keyboardCallback);

        // Load glad
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw std::runtime_error { "Failed to initalize GLAD" };
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
