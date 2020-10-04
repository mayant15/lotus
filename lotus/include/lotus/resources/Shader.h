#pragma once

#include "lotus/lcommon.h"
#include "lotus/physics/physics.h"
#include "lotus/ecs/ALight.h"
#include "lotus/resources/Material.h"

namespace Lotus
{
    struct LightInfo
    {
        Vector3f ambient = Vector3f(0.0f);
        Vector3f diffuse = Vector3f(1.0f);
        Vector3f specular = Vector3f(0.4f);
        Vector3f direction = Vector3f(1.0f);

        // TODO: Remove the default constructor? This should hold only temporary information
        // relevant for rendering that frame, and should hence be created by the component
        LightInfo() = default;
        explicit LightInfo(const CLight& light)
        : ambient(light.ambient),
          diffuse(light.diffuse),
          specular(light.specular)
        {}
    };

    struct PointLightInfo : LightInfo
    {
        float constant = 1.0f;
        float linear = 0.22f;
        float quadratic = 0.20f;
        Vector3f position = ORIGIN;

        PointLightInfo() = default;
        explicit PointLightInfo(const CPointLight& light)
                : LightInfo(light),
                  constant(light.constant),
                  linear(light.linear),
                  quadratic(light.quadratic)
        {}
    };

    struct SpotLightInfo : LightInfo
    {
        float constant = 1.0f;
        float linear = 0.07f;
        float quadratic = 0.017f;
        float innerCutOff = 12.5f;
        float outerCutOff = 17.5f;
        Vector3f position = ORIGIN;

        SpotLightInfo() = default;
        explicit SpotLightInfo(const CSpotlight& light)
                : LightInfo(light),
                  constant(light.constant),
                  linear(light.linear),
                  quadratic(light.quadratic),
                  innerCutOff(light.innerCutOff),
                  outerCutOff(light.outerCutOff)
        {}
    };

    struct LOTUS_API Shader
    {
        uint32_t ID;

        virtual void Use() const = 0;
        virtual void SetInt(const std::string& name, int value) const = 0;
        virtual void SetFloat(const std::string& name, float value) const = 0;
        virtual void SetBool(const std::string& name, bool value) const = 0;
        virtual void SetMat4f(const std::string& name, const Matrix4f& mat) const = 0;
        virtual void SetVec3f(const std::string& name, const Vector3f& vec) const = 0;
        virtual void SetPointLight(const std::string& name, const PointLightInfo& options) const = 0;
        virtual void SetSpotlight(const std::string& name, const SpotLightInfo& options) const = 0;
        virtual void SetDirectionalLight(const std::string& name, const LightInfo& options) const = 0;
        virtual void SetPointLightArray(const std::string& name, const std::vector<PointLightInfo>& lights) const = 0;
        virtual void SetSpotlightArray(const std::string& name, const std::vector<SpotLightInfo>& lights) const = 0;
        virtual void SetDirLightArray(const std::string& name, const std::vector<LightInfo>& lights) const = 0;
        virtual void SetMaterial(const std::string& name, Handle<Material> mat) const = 0;
        virtual ~Shader() = default;
    };

    LOADER(ShaderLoader, Shader)
    {
        SRef<Shader> Load(const std::string& vertexPath, const std::string& fragmentPath) const;
    };
}
