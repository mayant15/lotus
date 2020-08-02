#pragma once

#include <memory>
#include "scene.h"
#include "resources.h"
#include "config.h"

namespace Lotus
{
    class Renderer
    {
    private:
        Lotus::EContext context;
        Renderer() = default;
    public:
        Renderer(Lotus::EContext context_) : context(context_) {};
        Renderer(const Renderer& ) = delete;
        Renderer& operator=(const Renderer& ) = delete;

        void setViewport(int x, int y, int width, int height);
        void init(bool isDebug);
        void renderScene(const Lotus::Scene& scene);
        void shutdown();

        void update();
    };

    class Shader
    {
    public:

        unsigned int ID;

        Shader(const std::string& vertexPath, const std::string& fragmentPath);

        void use() const;

        void setBool(const std::string& name, bool value) const;

        void setInt(const std::string& name, int value) const;

        void setFloat(const std::string& name, float value) const;

        void setMat4f(const std::string& name, bool transpose, const Matrix4f& mat) const;

        void setVec3f(const std::string& name, const Vector3f& vec) const;

        void setMat3f(const std::string& name, int transpose, const Matrix3f& mat) const;

        void setPointLight(const std::string& name, const CPointLight& options) const;

        void setSpotlight(const std::string& name, const CSpotlight& options) const;

        void setDirectionalLight(const std::string& name, const CDirectionalLight& options) const;

        void setPointLightArray(const std::string& name, const std::vector<CPointLight>& lights) const;

        void setSpotlightArray(const std::string& name, const std::vector<CSpotlight>& lights) const;

        void setDirLightArray(const std::string& name, const std::vector<CDirectionalLight>& lights) const;
    };

    typedef std::shared_ptr<Shader> SRefShader;

    struct CModel : IComponent
    {
        SRefModel model;
        SRefShader shader;
    };

    class AModel : public AActor
    {
    public:
        CModel model;
        AModel(const Vector3f& position_, const SRefModel& model_, const SRefShader& shader_);
    };
}
