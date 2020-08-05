#include <GLFW/glfw3.h>

#include "lotus/lotus.h"
#include "lotus/debug.h"

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
    Lotus::Engine& engine = Lotus::Engine::Get();
    Lotus::LotusOp config = {
            .RenderAPI = Lotus::ERenderAPI::OPEN_GL,
            .IsDebug = true,
            .Width = 1024,
            .Height = 800
    };
    engine.Initialize(config);

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

    Lotus::SRef<Lotus::LShader> whiteShader = std::make_shared<Lotus::LShader>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/standard.vsh",
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/emission.fsh"
    );

    // Initialize and import the model
    Lotus::SRef<Lotus::LModel> model = std::make_shared<Lotus::LModel>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/mesh/untitled.obj"
    );
    model->import();

//    Lotus::SRefModel planeModel = std::make_shared<Lotus::Model>(
//            "/home/priyansh/code/lotus/examples/quickstart/resources/mesh/plane.obj"
//    );
//    planeModel->import();

    Lotus::SceneManager& sceneManager = Lotus::SceneManager::Get();
    const Lotus::URef<Lotus::Scene>& scene = sceneManager.LoadScene("");
    Lotus::AActor entity = scene->CreateActor(ORIGIN);
    Lotus::CMeshRenderer meshRenderer {
        .Shader = whiteShader,
        .Model = model
    };
    entity.AddComponent<Lotus::CMeshRenderer>(meshRenderer);

    scene->CreateCamera(5.0f * Z_AXIS, true);

    // Test out python script
    exec_file("/home/priyansh/code/lotus/examples/quickstart/resources/scripts/hello.py");

    engine.Run();

    // Run the main render loop


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
