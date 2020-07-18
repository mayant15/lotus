#pragma once

#include <string>
#include <glad/glad.h>

class Shader
{
public:

    unsigned int ID;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    void use() const;

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setMat4fv(const std::string &name, GLboolean transpose, const float *value_ptr) const;

    void setVec3f(const std::string& name, const float* value_ptr) const;

    void setMat3fv(const std::string& name, int transpose, const float* value_ptr) const;

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
