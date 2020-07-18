#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "lotus/ui.h"
#include "lotus/rendering.h"
#include "lotus/debug.h"
#include "lotus/scene.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;

int main()
{
    lotus::init();
    GLRenderer renderer;
    renderer.setViewport(0, 0, 800, 600);
    GLFWwindow* window = renderer.getActiveWindow();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    GUI gui(window, Context::OpenGL);

    // Capture mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Callback for mouse movements
//    glfwSetCursorPosCallback(window, mouse_callback);

    // Callback for scroll
//    glfwSetScrollCallback(window, scroll_callback);

    // Create the shader to be used
    // TODO: Change the path here to something on your PC
    Shader shader(
            RESOURCE("shaders/standard.vsh"),
            RESOURCE("shaders/diffuse.fsh")
    );

    Shader whiteShader(
            RESOURCE("shaders/standard.vsh"),
            RESOURCE("shaders/emission.fsh")
    );

//    Model backpack("/home/priyansh/code/learnopengl/resources/backpack/backpack.obj");
    Model cubeModel(RESOURCE("mesh/untitled.obj"));

    CMesh diffuseMesh(cubeModel, shader);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    CMesh whiteMesh(cubeModel, whiteShader);

    Scene scene;
    scene.addCamera(glm::vec3(0.0f, 0.0f, 5.0f));
    scene.addActor(glm::vec3(0.0f), diffuseMesh);
    scene.addLight(lightPos, whiteMesh);

    // Ambient light config
    float lightCol[] = {0.0f, 0.0f, 0.0f};
    float objectCol[] = {0.5f, 0.5f, 0.5f};
    float ambientStrength = 0.2f;
    float specularStrength = 0.5f;
    float theta = 60;

    // Run the main render loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        std::shared_ptr<Camera> camera = scene.getCamera();
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera->ProcessKeyboard(FORWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera->ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            camera->ProcessKeyboard(UP, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            camera->ProcessKeyboard(DOWN, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera->ProcessKeyboard(RIGHT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera->ProcessKeyboard(LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Start the Dear ImGui frame
        gui.newFrame();
        {
            // Window
            ImGui::Begin("Options");
            ImGui::ColorEdit3("Light", lightCol);
            ImGui::ColorEdit3("Cube", objectCol);
            ImGui::DragFloat("Ambient Strength", &ambientStrength, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Speed", &theta, 0.1f, 0.0f);
            ImGui::End();
        }

        // Rendering
        glClearColor(0.2f, 0.3f, 0.3, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

//        if (showWireframe)
//        {
//            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//        }
//        else
//        {
//            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//        }

        renderer.renderScene(scene);
        glCheckError();

        gui.render();
        glCheckError();

        glfwSwapBuffers(window);
        glCheckError();
    }

    gui.shutdown();
    ImGui::DestroyContext();
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
