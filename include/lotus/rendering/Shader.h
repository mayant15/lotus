#pragma once

#include <string>
#include <glad/glad.h>

class Shader
{
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath);

    void use() const;

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setMat4fv(const std::string &name, GLboolean transpose, const float *value_ptr) const;

    void setVec3f(const std::string& name, const float* value_ptr) const;

    void setMat3fv(const std::string& name, int transpose, const float* value_ptr) const;
};
