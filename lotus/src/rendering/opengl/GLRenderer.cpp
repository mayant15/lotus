#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

#include "glad/glad.h"
#include "lotus/debug.h"
#include "GLRenderer.h"

namespace Lotus
{
    void GLRenderer::Initialize(const Lotus::RendererOp& options)
    {
        // Initialize and configure GLFW
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, options.IsDebug);

        _options = options;
        _window = std::make_unique<Window>(options.RenderAPI, options.Width, options.Height, "Lotus - OpenGL");
        glfwSetFramebufferSizeCallback(_window->getGLWindow(), framebufferSizeCallback);


        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            LOG_ERROR("Failed to initialize GLAD");
            return;
        }

        // Enable depth options
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Setup framebuffer targets
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        // Create color buffer
        glGenTextures(1, &texColorBuffer);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, options.Width, options.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // attach it to the FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

        // create depth/stencil renderbuffer objects
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, options.Width, options.Height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // attach it to the FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        // setup completed?
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            LOG_ERROR("Framebuffer is not complete");
        }

        // Unbind the FBO now that everything has been set up
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Enable debug output
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            LOG_INFO("Setting up a debug context...");
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(debugMessageCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
    }

    unsigned int GLRenderer::createTexture(unsigned char* data, int width, int height, unsigned int format)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return textureID;
    }

    URef<Window>& GLRenderer::GetActiveWindow()
    {
        return _window;
    }

    void GLRenderer::SetViewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }

    void GLRenderer::DrawMesh(const CMeshRenderer& data, const CTransform& transform)
    {
        SRef<LShader> shader = data.Shader;
        shader->use();

        // Set transforms and draw actor
        Matrix4f model(1.0f);
        model = Lotus::LTranslate(model, transform.position);
        model = Lotus::LRotate(model, transform.rotation.x, X_AXIS);
        model = Lotus::LRotate(model, transform.rotation.y, Y_AXIS);
        model = Lotus::LRotate(model, transform.rotation.z, Z_AXIS);
        model = Lotus::LScale(model, transform.scale);
        shader->setMat4f("model", false, model);

        // Set camera
        shader->setMat4f("view", false, view);
        shader->setMat4f("projection", GL_FALSE, projection);
//        shader->setVec3f("viewPos", cameraPos);

        // Material. Diffuse is set through a texture
//        Vector3f specular(0.5f);
//        shader->setVec3f("material.specular", specular);
//        shader->setFloat("material.shininess", 32);

        std::vector<Lotus::Mesh> meshes = data.Model->getMeshes();
        for (Lotus::Mesh& mesh : meshes)
        {
            // Setup textures
//            unsigned int diffuseNum = 1;
//            unsigned int specularNum = 1;
//            for (unsigned int i = 0; i < mesh.textures.size(); ++i)
//            {
//                glActiveTexture(GL_TEXTURE0 + i);
//                std::string number;
//                std::string name = mesh.textures[i]->type;
//                if (name == DIFFUSE_TEXTURE)
//                {
//                    number = std::to_string(diffuseNum++);
//                }
//                else if (name == SPECULAR_TEXTURE)
//                {
//                    number = std::to_string(specularNum++);
//                }
//
//                shader->setInt("material." + name + number, i);
//                glBindTexture(GL_TEXTURE_2D, mesh.textures[i]->id);
//            }

            // Reset the active texture
//            glActiveTexture(GL_TEXTURE0);

            glBindVertexArray(mesh.VAO);

            glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }

    void GLRenderer::OnPreUpdate()
    {
        // TODO: Should be done by the event system
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        const URef<Scene>& scene = SceneManager::Get().GetActiveScene();
        ACamera camera = scene->GetActiveCamera();

        // Get camera properties
        view = camera.GetViewMatrix();
        cameraPos = camera.GetPosition();

        float aspectRatio = (float) _options.Width / _options.Height;
        projection = Lotus::LPerspective(glm::radians(camera.GetFieldOfView()), aspectRatio, 0.1f, 100.0f);

        // TODO: Get scene lighting data
    }

    void GLRenderer::OnUpdate(float delta)
    {
        const URef<Scene>& scene = SceneManager::Get().GetActiveScene();
        auto entityView = scene->Find<CMeshRenderer, CTransform>();
        for (auto entity : entityView)
        {
            const auto& [data, transform] = entityView.get<CMeshRenderer, CTransform>(entity);
            DrawMesh(data, transform);
        }
    }

    void GLRenderer::OnPostUpdate()
    {
        glfwSwapBuffers(_window->getGLWindow());
    }

    void GLRenderer::OnPreDestroy()
    {
        // TODO
    }

    void GLRenderer::OnDestroy()
    {
        glfwDestroyWindow(_window->getGLWindow());
        glfwTerminate();
    }

    void GLRenderer::framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void GLRenderer::debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                          const GLchar* message, const void* userParam)
    {
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
            return;
        std::string sourceString;
        std::string typeString;
        switch (source)
        {
            case GL_DEBUG_SOURCE_API:
                sourceString = "Source: API";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                sourceString = "Source: Window System";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                sourceString = "Source: Shader Compiler";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                sourceString = "Source: Third Party";
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                sourceString = "Source: Application";
                break;
            case GL_DEBUG_SOURCE_OTHER:
                sourceString = "Source: Other";
                break;
        }
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:
                typeString = "Type: Error";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                typeString = "Type: Deprecated Behaviour";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                typeString = "Type: Undefined Behaviour";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                typeString = "Type: Portability";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                typeString = "Type: Performance";
                break;
            case GL_DEBUG_TYPE_MARKER:
                typeString = "Type: Marker";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                typeString = "Type: Push Group";
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                typeString = "Type: Pop Group";
                break;
            case GL_DEBUG_TYPE_OTHER:
                typeString = "Type: Other";
                break;
        }
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                LOG_ERROR ("({}): {}. {}. {}", id, message, sourceString, typeString);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                LOG_WARN  ("({}): {}. {}. {}", id, message, sourceString, typeString);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                LOG_WARN  ("({}): {}. {}. {}", id, message, sourceString, typeString);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                LOG_INFO  ("({}): {}. {}. {}", id, message, sourceString, typeString);
                break;
        }
    }
}
#pragma clang diagnostic pop