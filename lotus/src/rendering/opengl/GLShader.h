#pragma once

#include <lotus/resources/Shader.h>

namespace Lotus
{
    struct GLShader final : Shader
    {
        GLShader(const std::string& vertexPath, const std::string& fragmentPath);

        void Use() const override;

        void SetInt(const std::string& name, int value) const override;

        void SetFloat(const std::string& name, float value) const override;

        void SetBool(const std::string& name, bool value) const override;

        void SetMat4f(const std::string& name, const Matrix4f& mat) const override;

        void SetVec3f(const std::string& name, const Vector3f& vec) const override;

        void SetColor(const std::string& name, const Color& vec) const override;

        void SetPointLight(const std::string& name, const PointLightInfo& options) const override;

        void SetSpotlight(const std::string& name, const SpotLightInfo& options) const override;

        void SetDirectionalLight(const std::string& name, const SunLightInfo& options) const override;

        void SetPointLightArray(const std::string& name, const std::vector<PointLightInfo>& lights) const override;

        void SetSpotlightArray(const std::string& name, const std::vector<SpotLightInfo>& lights) const override;

        void SetDirLightArray(const std::string& name, const std::vector<SunLightInfo>& lights) const override;

        void SetMaterial(const std::string& name, Handle<Material> mat) const override;

        void Compile() override;

        void Reload() override;

    private:
        std::string vpath;
        std::string fpath;
    };
}
