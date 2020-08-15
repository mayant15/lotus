#include "lotus/lotus.h"
#include "lotus/debug.h"

#include "CameraSystem.hpp"

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

    // Create the shaders to be used
    Lotus::SRef<Lotus::LShader> shader = std::make_shared<Lotus::LShader>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/standard.vsh",
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/diffuse.fsh"
    );

    Lotus::SRef<Lotus::LShader> whiteShader = std::make_shared<Lotus::LShader>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/standard.vsh",
            "/home/priyansh/code/lotus/examples/quickstart/resources/shaders/emission.fsh"
    );

    // Initialize and import the model
    Lotus::SRef<Lotus::LModel> model = std::make_shared<Lotus::LModel>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/mesh/untitled.obj"
    );
    model->import();

    Lotus::SRef<Lotus::LModel> planeModel = std::make_shared<Lotus::LModel>(
            "/home/priyansh/code/lotus/examples/quickstart/resources/mesh/plane.obj"
    );
    planeModel->import();

    Lotus::SceneManager& sceneManager = Lotus::SceneManager::Get();
    const Lotus::URef<Lotus::Scene>& scene = sceneManager.LoadScene("");

    // Box
    Lotus::AActor entity = scene->CreateActor(ORIGIN);
    Lotus::CMeshRenderer meshRenderer {
        .Shader = shader,
        .Model = model
    };
    entity.AddComponent<Lotus::CMeshRenderer>(meshRenderer);

    // Plane
    Lotus::AActor plane = scene->CreateActor(-2.0f * Y_AXIS);
    Lotus::CMeshRenderer planeRenderer {
            .Shader = shader,
            .Model = planeModel
    };
    plane.AddComponent<Lotus::CMeshRenderer>(planeRenderer);

    // Directional light
    Lotus::AActor dirLight = scene->CreateActor(5.0f * X_AXIS);
    Lotus::CDirectionalLight cDirectionalLight;
    cDirectionalLight.direction = Vector3f (-1.0f, -1.0f, 1.0f);
    dirLight.AddComponent<Lotus::CDirectionalLight>(cDirectionalLight);

    // Spotlight
    Lotus::AActor spotlight = scene->CreateActor(5.0f * Y_AXIS);
    Lotus::CSpotlight cSpotlight;
    cSpotlight.position = 5.0f * Y_AXIS;
    cSpotlight.direction = -1.0f * Y_AXIS;
    spotlight.AddComponent<Lotus::CSpotlight>(cSpotlight);

    // Point light
    Lotus::AActor pointLight = scene->CreateActor(3.0f * Y_AXIS - 2.0f * X_AXIS);
    Lotus::CPointLight cPointLight;
    cPointLight.position = 3.0f * Y_AXIS - 2.0f * X_AXIS;
    pointLight.AddComponent<Lotus::CPointLight>(cPointLight);

    // Camera
    Lotus::ACamera camera = scene->CreateCamera(10.0f * Z_AXIS + 5.0f * Y_AXIS, 45, true);
    camera.GetTransform().Rotation = Vector3f (-20, -90, 0);

    // Test out python script
    exec_file("/home/priyansh/code/lotus/examples/quickstart/resources/scripts/hello.py");

    // Bind event callbacks
    CameraSystem cameraSystem;
    Lotus::EventManager& em = Lotus::EventManager::Get();
    em.Bind<UpdateEvent, &CameraSystem::OnUpdate>(cameraSystem);
    em.Bind<MouseEvent, &CameraSystem::OnMouseEvent>(cameraSystem);

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
