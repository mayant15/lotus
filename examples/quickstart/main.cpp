#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "lotus/lotus.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;

namespace LR = Lotus::Rendering;

int main()
{
    // Have to initialize the engine first
    Lotus::init();

    // Then choose the renderer and initialize that
    LR::GLRenderer& renderer = LR::GLRenderer::get();
    renderer.init();

    // Set the viewport dimensions
    renderer.setViewport(0, 0, 800, 600);

    LR::URefWindow& window = renderer.getActiveWindow();

    // Capture mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Callback for mouse movements
//    glfwSetCursorPosCallback(window, mouse_callback);

    // Callback for scroll
//    glfwSetScrollCallback(window, scroll_callback);

    // Create the shaders to be used
    LR::SRefShader shader = std::make_shared<LR::Shader>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/standard.vsh",
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/diffuse.fsh"
    );

    LR::SRefShader whiteShader = std::make_shared<LR::Shader>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/standard.vsh",
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/emission.fsh"
    );

    // Initialize and import the model
    Lotus::Resource::SRefModel model = std::make_shared<Lotus::Resource::Model>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/mesh/untitled.obj"
    );
    model->import();

    Lotus::Scene scene;

    Lotus::SRefActor actor = std::make_shared<Lotus::Actor>(ORIGIN, model, shader);
    scene.addActor(actor);

    Lotus::SRefCamera camera = std::make_shared<Lotus::LCamera>(glm::vec3(0.0f, 0.0f, 5.0f));
    scene.addCamera(camera);

    Lotus::SRefALight light = std::make_shared<Lotus::ALight>(glm::vec3(2.0f, 2.0f, 2.0f), model, whiteShader);
    scene.addLight(light);

    // Run the main render loop
    GLFWwindow* pWindow = window->getGLWindow();
    while (!glfwWindowShouldClose(pWindow))
    {
        glfwPollEvents();
        if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera->ProcessKeyboard(FORWARD, deltaTime);
        }
        if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera->ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS)
        {
            camera->ProcessKeyboard(UP, deltaTime);
        }
        if (glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS)
        {
            camera->ProcessKeyboard(DOWN, deltaTime);
        }
        if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera->ProcessKeyboard(RIGHT, deltaTime);
        }
        if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera->ProcessKeyboard(LEFT, deltaTime);
        }
        if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(pWindow, true);
        }

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Rendering
        glClearColor(0.2f, 0.3f, 0.3, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        renderer.renderScene(scene);
        glfwSwapBuffers(pWindow);
    }

    renderer.shutdown();
    return 0;
}

/**
 * Function to process and consume input events.
 * @param window: pointer to the active window object
 */
//void process_input(GLFWwindow* window)
//{
//
//}
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//    static bool firstMouse = true;
//    if (firstMouse) // initially set to true
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos;
//    lastX = xpos;
//    lastY = ypos;
//
//    camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.ProcessMouseScroll(yoffset);
//}
