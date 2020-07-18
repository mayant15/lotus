#include <lotus/debug.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "lotus/rendering/Renderer.h"

GLRenderer::GLRenderer()
{
    // Initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window and an OpenGL context
    window = glfwCreateWindow(800, 600, "Title", nullptr, nullptr);
    if (window == nullptr)
    {
        LOG_ERROR("Failed to create window.");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD");
        return;
    }

    // Enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugMessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // Enable depth testing with the z-buffer
    glEnable(GL_DEPTH_TEST);

    // Enable stencil test
    glEnable(GL_STENCIL_TEST);
}

void GLRenderer::setViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void GLRenderer::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow *GLRenderer::getActiveWindow()
{
    return window;
}

void GLRenderer::shutdown()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GLRenderer::renderScene(const Scene& scene)
{
    // Get camera properties
    std::shared_ptr<Camera> camera = scene.getCamera();
    glm::mat4 view = camera->GetViewMatrix();
    glm::vec3 cameraPos = camera->getPosition();

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float width = viewport[2];
    float height = viewport[3];
    glm::mat4 projection = glm::perspective(camera->getFieldOfView(), width / height, 0.1f, 100.0f);

    // Get the first light
    CPointLight light = scene.getLights()[0]->light;

    // Render actors
    std::vector<std::shared_ptr<Actor>> actors = scene.getActors();
    for (const std::shared_ptr<Actor>& actor : actors) {
        if (actor->isActive) {
            // TODO: Cache these shaders somewhere? So that lighting and camera properties are set only once?
            std::shared_ptr<Shader> shader = actor->getMesh().shader;
            shader->use();

            // Set camera
            shader->setMat4fv("view", GL_FALSE, glm::value_ptr(view));
            shader->setMat4fv("projection", GL_FALSE, glm::value_ptr(projection));
            shader->setVec3f("viewPos", glm::value_ptr(cameraPos));

            // TODO: Improve light handling
            // Set lighting
            std::string name = "pointLight";
            shader->setFloat(name + ".constant", light.constant);
            shader->setFloat(name + ".linear", light.linear);
            shader->setFloat(name + ".quadratic", light.quadratic);
            shader->setVec3f(name + ".position", glm::value_ptr(light.position));
            shader->setVec3f(name + ".ambient", glm::value_ptr(light.ambient));
            shader->setVec3f(name + ".diffuse", glm::value_ptr(light.diffuse));
            shader->setVec3f(name + ".specular", glm::value_ptr(light.specular));

            // Set transforms and draw actor
            actor->update();
        }
    }
}
