#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#include "lotus/ui.h"
#include "lotus/rendering.h"
#include "lotus/debug.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void process_input(GLFWwindow*);

void mouse_callback(GLFWwindow* window, double, double);

void scroll_callback(GLFWwindow*, double, double);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;

// Create the camera to be used
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

int main()
{
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
            "/home/priyansh/code/lotus/lotus/src/rendering/shaders/vertex.glsl",
            "/home/priyansh/code/lotus/lotus/src/rendering/shaders/fragment.glsl"
    );

    Shader emissiveShader(
            "/home/priyansh/code/lotus/lotus/src/rendering/shaders/vertex.glsl",
            "/home/priyansh/code/lotus/lotus/src/rendering/shaders/emissiveFragment.glsl"
    );

//    Model backpack("/home/priyansh/code/learnopengl/resources/backpack/backpack.obj");
    Model cube("/home/priyansh/code/lotus/lotus/resources/cube.obj");

    bool showWireframe = false;
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
        shader.setMat4fv("model", GL_FALSE, glm::value_ptr(model));
        shader.setMat4fv("view", GL_FALSE, glm::value_ptr(view));
        shader.setMat4fv("projection", GL_FALSE, glm::value_ptr(projection));
        shader.setMat3fv("normalMatrix", GL_FALSE, glm::value_ptr(projection));
        shader.setVec3f("lightColor", lightCol);
        shader.setVec3f("objectColor", objectCol);

        float time =  glfwGetTime();
        float lightX = glm::cos(glm::radians(time * theta)) * 2;
        float lightY = 2.0f;
        float lightZ = glm::sin(glm::radians(time * theta)) * 2;
        shader.setVec3f("lightPos", glm::value_ptr(glm::vec3(lightX, lightY, lightZ)));
        shader.setVec3f("viewPos", glm::value_ptr(camera.getPosition()));
        shader.setFloat("ambientStrength", ambientStrength);
        shader.setFloat("specularStrength", specularStrength);
        cube.draw(shader);
        glCheckError();

        emissiveShader.use();
        model = glm::translate(glm::mat4(1.0f), glm::vec3(lightX, lightY, lightZ));
        model = glm::scale(model, glm::vec3(0.2f));
        normal = glm::mat3(glm::transpose(glm::inverse(model)));
        emissiveShader.setMat4fv("model", GL_FALSE, glm::value_ptr(model));
        emissiveShader.setMat4fv("view", GL_FALSE, glm::value_ptr(view));
        emissiveShader.setMat4fv("projection", GL_FALSE, glm::value_ptr(projection));
        cube.draw(emissiveShader);
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

#pragma clang diagnostic pop