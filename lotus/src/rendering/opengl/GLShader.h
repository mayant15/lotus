#pragma once

#include "lotus/resources/Shader.h"

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
        void SetPointLight(const std::string& name, const CPointLight& options) const override;
        void SetSpotlight(const std::string& name, const CSpotlight& options) const override;
        void SetDirectionalLight(const std::string& name, const CDirectionalLight& options) const override;
        void SetPointLightArray(const std::string& name, const std::vector<CPointLight>& lights) const override;
        void SetSpotlightArray(const std::string& name, const std::vector<CSpotlight>& lights) const override;
        void SetDirLightArray(const std::string& name, const std::vector<CDirectionalLight>& lights) const override;
        void SetMaterial(const std::string& name, Handle<Material> mat) const override;
    };
}

