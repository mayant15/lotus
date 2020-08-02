#pragma once

#include <memory>
#include <GLFW/glfw3.h>
#include "components.h"
#include "scene.h"
#include "resources.h"
#include "config.h"

namespace Lotus::Rendering
{
    class Shader
    {
    public:

        unsigned int ID;

        Shader(const std::string& vertexPath, const std::string& fragmentPath);

        void use() const;

        void setBool(const std::string& name, bool value) const;

        void setInt(const std::string& name, int value) const;

        void setFloat(const std::string& name, float value) const;

        void setMat4fv(const std::string& name, GLboolean transpose, const float* value_ptr) const;

        void setVec3f(const std::string& name, const float* value_ptr) const;

        void setMat3fv(const std::string& name, int transpose, const float* value_ptr) const;

        void setPointLight(const std::string& name, const CPointLight& options) const;

        void setSpotlight(const std::string& name, const CSpotlight& options) const;

        void setDirectionalLight(const std::string& name, const CDirectionalLight& options) const;

        void setPointLightArray(const std::string& name, const std::vector<CPointLight>& lights) const;

        void setSpotlightArray(const std::string& name, const std::vector<CSpotlight>& lights) const;

        void setDirLightArray(const std::string& name, const std::vector<CDirectionalLight>& lights) const;

    };

    typedef std::shared_ptr<Shader> SRefShader;

    class Window
    {
        GLFWwindow* pGLWindow;
    public:
        Window(Lotus::Context context, unsigned int width, unsigned int height, const std::string& title);

        GLFWwindow* getGLWindow();
    };

    typedef std::unique_ptr<Window> URefWindow;


    class IRenderer
    {
    protected:
        URefWindow window;

    public:
        virtual URefWindow& getActiveWindow() = 0;

        virtual void init(bool isDebug) = 0;

        virtual void renderScene(const Lotus::Scene& scene) = 0;

        virtual void shutdown() = 0;
    };

    class GLRenderer : public IRenderer
    {
        // Framebuffer object
        unsigned int FBO = 0;

        // Texture used as a color buffer
        unsigned int texColorBuffer = 0;

        // Render buffer object for depth and stencil buffers
        unsigned int RBO = 0;

        GLRenderer() = default;

        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                  const GLchar* message,
                                  const void* userParam);

        void renderModel(const Resource::SRefModel& model, const SRefShader& shader);

    public:

        GLRenderer(const GLRenderer&) = delete;

        GLRenderer& operator=(const GLRenderer&) = delete;

        static GLRenderer& get();

        void setViewport(int x, int y, int width, int height) const;

        URefWindow& getActiveWindow() override;

        void init(bool isDebug) override;

        void renderScene(const Scene& scene) override;

        void shutdown() override;

        unsigned int createTexture(unsigned char* data, int width, int height, unsigned int format);

    };

    class VkRenderer : public IRenderer
    {
    public:
        VkRenderer();
    };

    class DxRenderer : public IRenderer
    {
    public:
        DxRenderer();
    };
}
