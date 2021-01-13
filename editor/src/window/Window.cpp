#include "Window.h"

#include <config.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

namespace Editor
{
    static void glfwErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
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

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw std::exception("Failed to initalize GLAD");
        }

        return window;
    }

    void StartFrame(Window* window)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
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
