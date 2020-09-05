#pragma once

#include "lotus/lcommon.h"
#include "lotus/physics.h"
#include "lotus/components/lights.h"
#include "lotus/resources/Material.h"

namespace Lotus
{
    struct LOTUS_API Shader
    {
        uint32_t ID;

        virtual void Use() const = 0;
        virtual void SetInt(const std::string& name, int value) const = 0;
        virtual void SetFloat(const std::string& name, float value) const = 0;
        virtual void SetBool(const std::string& name, bool value) const = 0;
        virtual void SetMat4f(const std::string& name, const Matrix4f& mat) const = 0;
        virtual void SetVec3f(const std::string& name, const Vector3f& vec) const = 0;
        virtual void SetPointLight(const std::string& name, const CPointLight& options) const = 0;
        virtual void SetSpotlight(const std::string& name, const CSpotlight& options) const = 0;
        virtual void SetDirectionalLight(const std::string& name, const CDirectionalLight& options) const = 0;
        virtual void SetPointLightArray(const std::string& name, const std::vector<CPointLight>& lights) const = 0;
        virtual void SetSpotlightArray(const std::string& name, const std::vector<CSpotlight>& lights) const = 0;
        virtual void SetDirLightArray(const std::string& name, const std::vector<CDirectionalLight>& lights) const = 0;
        virtual void SetMaterial(const std::string& name, Handle<Material> mat) const = 0;
        virtual ~Shader() = default;
    };

    LOADER(ShaderLoader, Shader)
    {
        SRef<Shader> Load(const std::string& vertexPath, const std::string& fragmentPath) const;
    };
}
