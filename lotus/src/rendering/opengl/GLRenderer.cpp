#include "lotus/debug.h"
#include "GLRenderer.h"
#include "GLShader.h"
#include "lotus/resources/Model.h"
#include "lotus/scene/SceneManager.h"
#include "lotus/scene/ACamera.h"

namespace Lotus
{
    void GLRenderer::Initialize(const RendererOp& options)
    {
        _options = options;

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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, options.ViewportWidth, options.ViewportHeight, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // attach it to the FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

        // create depth/stencil renderbuffer objects
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, options.ViewportWidth, options.ViewportHeight);
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

    void GLRenderer::SetViewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }

    void GLRenderer::DrawMesh(const CMeshRenderer& data, const CTransform& transform)
    {
        Handle<Shader> shader = data.Shader;
        shader->Use();

        // Set transforms and draw actor
        Matrix4f model(1.0f);
        model = LTranslate(model, transform.Position);
        model = LRotate(model, transform.Rotation.x, X_AXIS);
        model = LRotate(model, transform.Rotation.y, Y_AXIS);
        model = LRotate(model, transform.Rotation.z, Z_AXIS);
        model = LScale(model, transform.Scale);
        shader->SetMat4f("model", model);

        // Set camera
        shader->SetMat4f("view", view);
        shader->SetMat4f("projection", projection);
        shader->SetVec3f("viewPos", cameraPos);


        // Set lighting
        shader->SetDirLightArray("dirLight", dirLightParams);
        shader->SetPointLightArray("pointLight", ptLightParams);
        shader->SetSpotlightArray("spotlight", spLightParams);

        std::vector<SubMesh> meshes = data.Model->Meshes;
        for (SubMesh& mesh : meshes)
        {
            // Material

            // Set diffuse texture
            auto diffuse = mesh.Material->Diffuse;
            if (std::holds_alternative<Vector3f>(diffuse))
            {
                // invalid texture, set the color
                shader->SetVec3f("material.diffuseColor", std::get<Vector3f>(diffuse));
            }
            else
            {
                // valid texture, set texture
                Handle<Texture> texture = std::get<Handle<Texture>>(diffuse);
                glActiveTexture(GL_TEXTURE0);
                shader->SetInt("material.texture_diffuse1", 0);
                glBindTexture(GL_TEXTURE_2D, texture->ID);
            }

            // Set specular texture
            auto specular = mesh.Material->Specular;
            if (std::holds_alternative<Vector3f>(specular))
            {
                // invalid texture, set the color
                shader->SetVec3f("material.specularColor", std::get<Vector3f>(specular));
            }
            else
            {
                // valid texture, set texture
                Handle<Texture> texture = std::get<Handle<Texture>>(specular);
                glActiveTexture(GL_TEXTURE1);
                shader->SetInt("material.texture_specular1", 1);
                glBindTexture(GL_TEXTURE_2D, texture->ID);
            }

            shader->SetFloat("material.shininess", mesh.Material->Shininess);

            // Reset the active texture
            glActiveTexture(GL_TEXTURE0);

            glBindVertexArray(mesh.VAO);

            glDrawElements(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }

    void GLRenderer::OnPreUpdate(const PreUpdateEvent& event)
    {
        glClearColor(0.2f, 0.3f, 0.8f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        const URef<Scene>& scene = SceneManager::Get().GetActiveScene();
        ACamera camera = scene->GetActiveCamera();

        // Get camera properties
        view = camera.GetViewMatrix();
        cameraPos = camera.GetAbsolutePosition();

        const float aspectRatio = (float) _options.ViewportWidth / _options.ViewportHeight;
        projection = LPerspective(glm::radians(camera.GetFieldOfView()), aspectRatio, 0.1f, 100.0f);

        glCheckError();
    }

    void GLRenderer::OnUpdate(const UpdateEvent& event)
    {
        const URef<Scene>& scene = SceneManager::Get().GetActiveScene();

        // Process lighting
        ptLightParams.clear();
        auto ptView = scene->Find<CPointLight>();
        for (auto light : ptView)
        {
            const auto& params = ptView.get<CPointLight>(light);
            ptLightParams.push_back(params);
        }

        spLightParams.clear();
        auto spView = scene->Find<CSpotlight>();
        for (auto light : spView)
        {
            const auto& params = spView.get<CSpotlight>(light);
            spLightParams.push_back(params);
        }

        dirLightParams.clear();
        auto dirView = scene->Find<CDirectionalLight>();
        for (auto light : dirView)
        {
            const auto& params = dirView.get<CDirectionalLight>(light);
            dirLightParams.push_back(params);
        }

        auto entityView = scene->Find<CMeshRenderer, CTransform>();
        for (auto entity : entityView)
        {
            glCheckError();
            const auto&[data, transform] = entityView.get<CMeshRenderer, CTransform>(entity);
            DrawMesh(data, transform);
            glCheckError();
        }

        // Draw skybox
        auto skyView = scene->Find<CSkybox>();
        const auto& sky = skyView.get<CSkybox>(skyView.front());

        glDepthFunc(GL_LEQUAL);
        sky.Shader->Use();
        sky.Shader->SetMat4f("view", Matrix4f (Matrix3f (view)));
        sky.Shader->SetMat4f("projection", projection);

        // skybox cube
        glBindVertexArray(sky.Map->VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, sky.Map->ID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
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
