#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "lotus/lotus.h"
#include "lotus/debug.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;

#define glCheckError() glCheckError_(__FILE__, __LINE__)

/**
 * Print out an error message with file name and line number
 */
GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
        LOG_ERROR("{} | {} ( {} )", error, file, line);
    }
    return errorCode;
}

int main()
{
    // Have to initialize the engine first
    Lotus::init();

    // Then choose the renderer and initialize that
    Lotus::Renderer renderer(Lotus::EContext::OPEN_GL);
    renderer.init(true);

    glCheckError();

    // Set the viewport dimensions
    renderer.setViewport(0, 0, 800, 600);

    // Capture mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Callback for mouse movements
//    glfwSetCursorPosCallback(window, mouse_callback);

    // Callback for scroll
//    glfwSetScrollCallback(window, scroll_callback);

    // Create the shaders to be used
    Lotus::SRefShader shader = std::make_shared<Lotus::Shader>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/standard.vsh",
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/diffuse.fsh"
    );

    Lotus::SRefShader whiteShader = std::make_shared<Lotus::Shader>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/standard.vsh",
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/emission.fsh"
    );

    // Initialize and import the model
    Lotus::SRefModel model = std::make_shared<Lotus::Model>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/mesh/untitled.obj"
    );
    model->import();

    Lotus::SRefModel planeModel = std::make_shared<Lotus::Model>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/mesh/plane.obj"
    );
    planeModel->import();

//    Lotus::Resource::SRefModel backpackModel = std::make_shared<Lotus::Resource::Model>(
//            "/home/priyansh/Desktop/backpack/backpack.obj"
//    );
//    backpackModel->import();

    glCheckError();

    Lotus::Scene scene;

    // Actors
//    Lotus::SRefActor actor = std::make_shared<Lotus::AActor>(ORIGIN, model, shader);
//    scene.addActor(actor);
//
//    Lotus::SRefActor actor2 = std::make_shared<Lotus::AActor>(glm::vec3(2.0f, 2.0f, 0.0f), model, shader);
//    scene.addActor(actor2);
//
//    Lotus::SRefActor plane = std::make_shared<Lotus::AActor>(glm::vec3(0.0f, -1.11f, 0.0f), planeModel, shader);
//    scene.addActor(plane);

    // Camera
    Lotus::SRefCamera camera = std::make_shared<Lotus::ACamera>(glm::vec3(0.0f, 0.0f, 5.0f));
    scene.addCamera(camera);

    // Lights
    Lotus::CSpotlight light;
    light.diffuse = glm::vec3(0.9375f, 0.99375f, 0.148f); // yellow
    light.ambient = glm::vec3(0.2f);
    light.specular = glm::vec3(0.5f);
    light.constant = 1.0f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    light.innerCutOff = glm::cos(glm::radians(30.0f));
    light.outerCutOff = glm::cos(glm::radians(32.5f));

//    light.direction = glm::vec3(-1.0f, -1.0f, 0.0f);
//    Lotus::SRefALight light1 = std::make_shared<Lotus::ALight>(light, model, whiteShader);
//    light1->transform.scale = glm::vec3 (0.2f, 0.2f, 0.2f);
//    scene.addLight(light1, ELight::DIR);

    light.direction = glm::vec3(-0.2f, -1.0f, 0.0f);
    light.position = glm::vec3(3.0f, 5.0f, 0.0f);
    Lotus::SRefALight light3 = std::make_shared<Lotus::ALight>(light);
    light3->transform.scale = glm::vec3 (0.2f, 0.2f, 0.2f);
    scene.addLight(light3, ELight::SPOT);

    light.position = glm::vec3(-2.0f, 2.0f, 2.0f);
    Lotus::SRefALight light4 = std::make_shared<Lotus::ALight>(light);
    light4->transform.scale = glm::vec3 (0.2f, 0.2f, 0.2f);
    scene.addLight(light4, ELight::POINT);

    exec_file("/home/priyansh/code/lotus/examples/quickstart/resources/scripts/hello.py");

    // Run the main render loop
    Lotus::run();
    double start_time = glfwGetTime();
    double current_time = start_time;
    while (current_time - start_time < 5)
    {
        current_time = glfwGetTime();
//        if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
//        {
//            camera->ProcessKeyboard(FORWARD, deltaTime);
//        }
//        if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
//        {
//            camera->ProcessKeyboard(BACKWARD, deltaTime);
//        }
//        if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS)
//        {
//            camera->ProcessKeyboard(UP, deltaTime);
//        }
//        if (glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS)
//        {
//            camera->ProcessKeyboard(DOWN, deltaTime);
//        }
//        if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
//        {
//            camera->ProcessKeyboard(RIGHT, deltaTime);
//        }
//        if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
//        {
//            camera->ProcessKeyboard(LEFT, deltaTime);
//        }
//        if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        {
//            glfwSetWindowShouldClose(pWindow, true);
//        }
//
//        float currentFrame = glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;

        // Rendering
        renderer.renderScene(scene);
        renderer.update();
    }

    glCheckError();
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
