#pragma once

#include "lotus/physics.h"
#include "lotus/components.h"

namespace Lotus
{
    class LShader
    {
    public:

        unsigned int ID;

        LShader(const std::string& vertexPath, const std::string& fragmentPath);

        void use() const;

        void setBool(const std::string& name, bool value) const;

        void setInt(const std::string& name, int value) const;

        void setFloat(const std::string& name, float value) const;

        void setMat4f(const std::string& name, bool transpose, const Matrix4f& mat) const;

        void setVec3f(const std::string& name, const Vector3f& vec) const;

        void setMat3f(const std::string& name, bool transpose, const Matrix3f& mat) const;

        void setPointLight(const std::string& name, const CPointLight& options) const;

        void setSpotlight(const std::string& name, const CSpotlight& options) const;

        void setDirectionalLight(const std::string& name, const CDirectionalLight& options) const;

        void setPointLightArray(const std::string& name, const std::vector<CPointLight>& lights) const;

        void setSpotlightArray(const std::string& name, const std::vector<CSpotlight>& lights) const;

        void setDirLightArray(const std::string& name, const std::vector<CDirectionalLight>& lights) const;
    };
}
