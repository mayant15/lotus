#include "GLRenderer.h"

#include "lotus/resources/HDRI.h"
#include "lotus/ecs/Entity.h"
#include "lotus/ecs/components/CCamera.h"
#include "lotus/debug.h"

#include "rendering/Primitives.h"

constexpr unsigned int SHADOW_WIDTH = 1024;
constexpr unsigned int SHADOW_HEIGHT = 1024;

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

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);

        // Enable backface culling
        glEnable(GL_CULL_FACE);

        // Enable MSAA
        glEnable(GL_MULTISAMPLE);

        // TODO: Alpha blending?
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Setup FBO for shadow maps
        glGenFramebuffers(1, &_shadowFBO);
        glGenTextures(1, &_shadowDepthTexture);
        glBindTexture(GL_TEXTURE_2D, _shadowDepthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

        glBindFramebuffer(GL_FRAMEBUFFER, _shadowFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadowDepthTexture, 0);

        // explicitly tell OpenGL that we're not going to attach a color buffer to this FBO
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);  

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

        // Setup internal shaders
        _shadowShader = LoadAsset<Shader, ShaderLoader>(INTERNAL_SHADERS("shadow"));
        _skyShader = LoadAsset<Shader, ShaderLoader>(INTERNAL_SHADERS("skybox"));
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
        shader->SetMat4f("lightView", lightView);
        shader->SetMat4f("lightProjection", lightProjection);
        shader->SetVec3f("camPos", cameraPos);

        // Set lighting
        shader->SetDirLightArray("dirLight", dirLightParams);
        shader->SetInt("numDirLight", dirLightParams.size());

        shader->SetPointLightArray("pointLight", ptLightParams);
        shader->SetInt("numPointLight", ptLightParams.size());

        shader->SetSpotlightArray("spotlight", spLightParams);
        shader->SetInt("numSpotlight", spLightParams.size());

        auto registry = GetRegistry();
        auto skyView = registry->view<CSkybox>();
        if (!skyView.empty())
        {
            const auto& sky = skyView.get<CSkybox>(skyView.front());

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, sky.Map->Irradiance);
            shader->SetInt("irradianceMap", 1);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _shadowDepthTexture);
        shader->SetInt("shadowMap", 0);

        std::vector<SubMesh> meshes = data.Model->Meshes;
        for (SubMesh& mesh : meshes)
        {
            // Material
            Handle<Material> material = mesh.Material;
            shader->SetMaterial("material", material);

            glBindVertexArray(mesh.VAO);
            glDrawElements(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }

    void GLRenderer::OnPreUpdate(const PreUpdateEvent& event)
    {
        // glClearColor(0.74f, 0.74f, 0.74f, 0.5f);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glCheckError();

        // TODO: Custom view function
        auto registry = GetRegistry();

        // Process lighting
        ptLightParams.clear();
        auto ptView = registry->view<CPointLight, CTransform>();
        for (auto light : ptView)
        {
            const auto& [params, transform] = ptView.get<CPointLight, CTransform>(light);
            PointLightInfo info (params);
            info.position = transform.Position;
            ptLightParams.push_back(info);
        }

        spLightParams.clear();
        auto spView = registry->view<CSpotlight, CTransform>();
        for (auto light : spView)
        {
            const auto& [params, transform] = spView.get<CSpotlight, CTransform>(light);
            SpotLightInfo info (params);
            info.position = transform.Position;
            info.direction = GetForwardVector(transform);
            spLightParams.push_back(info);
        }

        dirLightParams.clear();
        auto dirView = registry->view<CLight, CTransform>();
        for (auto light : dirView)
        {
            const auto& [params, transform] = dirView.get<CLight, CTransform>(light);
            LightInfo info (params);
            info.direction = GetForwardVector(transform);
            dirLightParams.push_back(info);
        }

        // Setup light view proj for shadow map
        if (!dirView.empty())
        {
            const auto& [param, transform] = dirView.get<CLight, CTransform>(dirView.front());

            // This light's direction is calculated from its forward vector
            Vector3f forward = GetForwardVector(transform);
            lightView = LLookAt(transform.Position, transform.Position + forward * 10.0f, UP);
            lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
        }

        auto cameraView = registry->view<CCamera, CTransform>();
        for (auto entity : cameraView)
        {
            const auto& [camera, transform] = cameraView.get<CCamera, CTransform>(entity);
            if (camera.IsActive)
            {
                const float aspectRatio = (float) _options.ViewportWidth / _options.ViewportHeight;

                cameraPos = transform.Position;
                view = GetViewMatrix(transform);
                projection = LPerspective(glm::radians(camera.FOV), aspectRatio, 0.1f, 100.0f);

                // Set the results of the first active camera that you find, then break
                break;
            }
        }
    }

    void GLRenderer::OnUpdate(const UpdateEvent& event)
    {
        // TODO: Bake the shadow map, don't generate it on update

        // Render shadow map for the first directional light in the scene
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, _shadowFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        
        _shadowShader->Use();
        _shadowShader->SetMat4f("lightView", lightView);
        _shadowShader->SetMat4f("lightProjection", lightProjection);

        auto registry = GetRegistry();
        
        auto entityView = registry->view<CMeshRenderer, CTransform>();
        for (auto entity : entityView)
        {
            const auto&[data, transform] = entityView.get<CMeshRenderer, CTransform>(entity);
            Matrix4f model(1.0f);
            model = LTranslate(model, transform.Position);
            model = LRotate(model, transform.Rotation.x, X_AXIS);
            model = LRotate(model, transform.Rotation.y, Y_AXIS);
            model = LRotate(model, transform.Rotation.z, Z_AXIS);
            model = LScale(model, transform.Scale);
            _shadowShader->SetMat4f("model", model);
        
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _shadowDepthTexture);

            for (const auto& mesh : data.Model->Meshes)
            {
                glBindVertexArray(mesh.VAO);
                glDrawElements(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, nullptr);
            }

            glBindVertexArray(0);
        }

        // Render scene as normal with the generated shadow map
        glViewport(0, 0, _options.ViewportWidth, _options.ViewportHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);

        for (auto entity : entityView)
        {
            glCheckError();
            const auto&[data, transform] = entityView.get<CMeshRenderer, CTransform>(entity);
            DrawMesh(data, transform);
            glCheckError();
        }

        // Draw skybox
        auto skyView = registry->view<CSkybox>();
        if (!skyView.empty())
        {
            const auto& sky = skyView.get<CSkybox>(skyView.front());

            glDepthFunc(GL_LEQUAL);
            _skyShader->Use();
            _skyShader->SetMat4f("view", Matrix4f (Matrix3f (view)));
            _skyShader->SetMat4f("projection", projection);

            // skybox cube
            glBindVertexArray(RHI::GetCubeVAO());
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, sky.Map->EnvironmentMap);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS); // set depth function back to default
        }
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
            default:
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
            default:
                typeString = "Type: Other";
                break;
        }
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                LOG_ERROR ("({}): {}. {}. {}", id, message, sourceString, typeString);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
            case GL_DEBUG_SEVERITY_LOW:
                LOG_WARN  ("({}): {}. {}. {}", id, message, sourceString, typeString);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
            default:
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
