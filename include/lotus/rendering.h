#pragma once

#include <memory>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
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

        // TODO: Set lights

//    void setPointLight(const std::string& name, const * options) const;
//
//    void setSpotlight(const std::string& name, const Spotlight& options) const;
//
//    void setPointLightArray(const std::string& name, const PointLight* lights, unsigned int length) const;
//
//    void setSpotlightArray(const std::string& name, const Spotlight* lights, unsigned int length) const;
//
//    void setDirectionalLight(const std::string& name, const DirectionalLight& options) const;
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

        virtual void init() = 0;

        virtual void renderScene(const Lotus::Scene& scene) = 0;

        virtual void shutdown() = 0;
    };

    class GLRenderer : public IRenderer
    {

        GLRenderer() = default;

        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    private:

        void renderModel(const Resource::SRefModel& model, const SRefShader& shader);

    public:

        GLRenderer(const GLRenderer&) = delete;

        GLRenderer& operator=(const GLRenderer&) = delete;

        static GLRenderer& get();

        void setViewport(int x, int y, int width, int height) const;

        URefWindow& getActiveWindow() override;

        void init() override;

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
