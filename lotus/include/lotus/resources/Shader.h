#pragma once

#include "lotus/lcommon.h"
#include "lotus/physics/Vector.h"
#include "lotus/ecs/components/CLight.h"
#include "lotus/resources/AssetRegistry.h"

namespace Lotus
{
    struct SunLightInfo
    {
        Color color;
        Vector3f direction;
    };

    struct PointLightInfo
    {
        Color color;
        float constant;
        float linear;
        float quadratic;
        Vector3f position;
    };

    struct SpotLightInfo
    {
        Color color;
        float constant;
        float linear;
        float quadratic;
        float innerCutOff;
        float outerCutOff;
        Vector3f position;
        Vector3f direction;
    };

    struct Material; // Have to forward declare Material for SetMaterial() to work
    struct LOTUS_API Shader
    {
        uint32_t ID;

        virtual void Use() const = 0;

        virtual void SetInt(const std::string& name, int value) const = 0;

        virtual void SetFloat(const std::string& name, float value) const = 0;

        virtual void SetBool(const std::string& name, bool value) const = 0;

        virtual void SetMat4f(const std::string& name, const Matrix4f& mat) const = 0;

        virtual void SetVec3f(const std::string& name, const Vector3f& vec) const = 0;

        virtual void SetColor(const std::string& name, const Color& vec) const = 0;

        virtual void SetPointLight(const std::string& name, const PointLightInfo& options) const = 0;

        virtual void SetSpotlight(const std::string& name, const SpotLightInfo& options) const = 0;

        virtual void SetDirectionalLight(const std::string& name, const SunLightInfo& options) const = 0;

        virtual void SetPointLightArray(const std::string& name, const std::vector<PointLightInfo>& lights) const = 0;

        virtual void SetSpotlightArray(const std::string& name, const std::vector<SpotLightInfo>& lights) const = 0;

        virtual void SetDirLightArray(const std::string& name, const std::vector<SunLightInfo>& lights) const = 0;

        virtual void SetMaterial(const std::string& name, Handle <Material> mat) const = 0;

        virtual void Compile() = 0;

        virtual void Reload() = 0;

        virtual ~Shader() = default;
    };

    LOADER(Shader)
    {
        [[nodiscard]] SRef <Shader> Load(const std::string& vertexPath, const std::string& fragmentPath) const;
    };
}
