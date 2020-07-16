#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#include "lotus/ui.h"
#include "lotus/rendering.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void process_input(GLFWwindow*);

void mouse_callback(GLFWwindow* window, double, double);

void scroll_callback(GLFWwindow*, double, double);


#define glCheckError() glCheckError_(__FILE__, __LINE__)
#define RESOURCE(x) lotus::resource(__FILE__, x)
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
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;

// Create the camera to be used
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

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

//    Model backpack("/home/priyansh/code/learnopengl/resources/backpack/backpack.obj");
    Model cube(RESOURCE("mesh/untitled.obj"));


    bool showWireframe = false;
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    // Ambient light config
    float lightCol[] = {0.0f, 0.0f, 0.0f};
    float objectCol[] = {0.5f, 0.5f, 0.5f};
    float ambientStrength = 0.2f;
    float specularStrength = 0.5f;

    float theta = 60;
    std::cout << camera.getFront()[1] << std::endl;
    // Run the main render loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        process_input(window);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Start the Dear ImGui frame
        gui.newFrame();
        {

            ImGui::Begin("Options");
            ImGui::Checkbox("Wireframe", &showWireframe);
            ImGui::ColorEdit3("Light", lightCol);
            ImGui::ColorEdit3("Cube", objectCol);
            ImGui::DragFloat("Ambient Strength", &ambientStrength, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Speed", &theta, 0.1f, 0.0f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering

        glClearColor(0.2f, 0.3f, 0.3, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (showWireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }


        // Set the matrices as uniforms for our shader.
        glm::mat4 model(1.0f);
        // Normal matrix handles adjustment of normals when objects are scaled
        glm::mat4 normal = glm::mat3(glm::transpose(glm::inverse(model)));

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        shader.use();

        // Transforms
        shader.setMat4fv("model", GL_FALSE, glm::value_ptr(model));
        shader.setMat4fv("view", GL_FALSE, glm::value_ptr(view));
        shader.setMat4fv("projection", GL_FALSE, glm::value_ptr(projection));
        shader.setMat3fv("normalMatrix", GL_FALSE, glm::value_ptr(projection));

        shader.setVec3f("viewPos", glm::value_ptr(camera.getPosition()));

        // Material. Diffuse is set through a texture
        shader.setVec3f("material.specular", objectCol);
        shader.setFloat("material.shininess", 32);

        // Light
        float time = glfwGetTime();
        float lightX = glm::cos(glm::radians(time * theta)) * 2;
        float lightY = 2.0f;
        float lightZ = glm::sin(glm::radians(time * theta)) * 2;

        DirectionalLight dirLight{};
        dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
        dirLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
        dirLight.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
        dirLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);
        shader.setDirectionalLight("dirLight", dirLight);

        PointLight light[1];
        light[0].position = glm::vec3(lightX, lightY, lightZ);
        light[0].diffuse = glm::vec3(lightCol[0], lightCol[1], lightCol[2]);
        light[0].ambient = glm::vec3(ambientStrength);
        light[0].specular = glm::vec3(specularStrength);
        light[0].constant = 1.0f;
        light[0].linear = 0.09f;
        light[0].quadratic = 0.032f;
        shader.setPointLightArray("pointLight", light, 1);

        Spotlight spotlight[1];
        spotlight[0].position = camera.getPosition();
        spotlight[0].direction = camera.getFront();
        spotlight[0].diffuse = glm::vec3(1.0f);
        spotlight[0].ambient = glm::vec3(0.0f);
        spotlight[0].specular = glm::vec3(1.0f);
        spotlight[0].constant = 1.0f;
        spotlight[0].linear = 0.09f;
        spotlight[0].quadratic = 0.032f;
        spotlight[0].innerCutOff = glm::cos(glm::radians(12.5f));
        spotlight[0].outerCutOff = glm::cos(glm::radians(17.5f));
        shader.setSpotlightArray("spotlight", spotlight, 1);

        cube.draw(shader);
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
void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse = true;
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
