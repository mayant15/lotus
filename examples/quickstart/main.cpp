#include <GLFW/glfw3.h>
#include <chrono>
#include "lotus/lotus.h"
#include "lotus/debug.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;

//#define glCheckError() glCheckError_(__FILE__, __LINE__)
//
///**
// * Print out an error message with file name and line number
// */
//GLenum glCheckError_(const char* file, int line)
//{
//    GLenum errorCode;
//    while ((errorCode = glGetError()) != GL_NO_ERROR)
//    {
//        std::string error;
//        switch (errorCode)
//        {
//            case GL_INVALID_ENUM:
//                error = "INVALID_ENUM";
//                break;
//            case GL_INVALID_VALUE:
//                error = "INVALID_VALUE";
//                break;
//            case GL_INVALID_OPERATION:
//                error = "INVALID_OPERATION";
//                break;
//            case GL_STACK_OVERFLOW:
//                error = "STACK_OVERFLOW";
//                break;
//            case GL_STACK_UNDERFLOW:
//                error = "STACK_UNDERFLOW";
//                break;
//            case GL_OUT_OF_MEMORY:
//                error = "OUT_OF_MEMORY";
//                break;
//            case GL_INVALID_FRAMEBUFFER_OPERATION:
//                error = "INVALID_FRAMEBUFFER_OPERATION";
//                break;
//        }
//        LOG_ERROR("{} | {} ( {} )", error, file, line);
//    }
//    return errorCode;
//}

int main()
{
    // Have to initialize the engine first
    Lotus::init();

    // Then choose the renderer and initialize that
    Lotus::Renderer& renderer = Lotus::Renderer::get();
    renderer.init(Lotus::EContext::OPEN_GL, true);

    // Set the viewport dimensions
    renderer.setViewport(0, 0, 1024, 600);

    // Capture mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Callback for mouse movements
//    glfwSetCursorPosCallback(window, mouse_callback);

    // Callback for scroll
//    glfwSetScrollCallback(window, scroll_callback);

    // Create the shaders to be used
//    Lotus::SRefShader shader = std::make_shared<Lotus::Shader>(
//            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/standard.vsh",
//            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/diffuse.fsh"
//    );

    Lotus::SRefShader whiteShader = std::make_shared<Lotus::Shader>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/standard.vsh",
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/emission.fsh"
    );

    // Initialize and import the model
    Lotus::SRefModel model = std::make_shared<Lotus::Model>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/mesh/untitled.obj"
    );
    model->import();

//    Lotus::SRefModel planeModel = std::make_shared<Lotus::Model>(
//            "/home/priyansh/code/lotus/examples/quickstart/resources/mesh/plane.obj"
//    );
//    planeModel->import();

    Lotus::Scene scene;

    Lotus::SRefAActor root = scene.getRoot();

    // Actors
    Lotus::SRefAModel box = std::make_shared<Lotus::AModel>(ORIGIN, model, whiteShader);
    root->addChild(box);

    // Camera
    Lotus::SRefACamera camera = std::make_shared<Lotus::ACamera>(Vector3f(0.0f, 0.0f, 5.0f));

    // Test out python script
    exec_file("/home/priyansh/code/lotus/examples/quickstart/resources/scripts/hello.py");

    // Run the main render loop
    double start_time = glfwGetTime();
    double current_time = start_time;
    Lotus::run();
    while (current_time - start_time < 5)
    {
        current_time = glfwGetTime();

        // Rendering
        glfwPollEvents();
        renderer.prepareFrame(camera);
        scene.update();
        renderer.swapBuffer();

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
