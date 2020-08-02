#include <fstream>
#include <string>
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"
#include "lotus/rendering.h"
#include "lotus/debug.h"

namespace Lotus::Rendering
{
    Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;

        try
        {
            std::ifstream vertexFile(vertexPath);
            vertexCode.assign((std::istreambuf_iterator<char>(vertexFile)), (std::istreambuf_iterator<char>()));

            std::ifstream fragmentFile(fragmentPath);
            fragmentCode.assign((std::istreambuf_iterator<char>(fragmentFile)), (std::istreambuf_iterator<char>()));

            vertexFile.close();
            fragmentFile.close();
        }
        catch (...)
        {
            LOG_ERROR("Shader Program: File not read.");
        }
        const char* vertexCodeStr = vertexCode.c_str();
        const char* fragmentCodeStr = fragmentCode.c_str();

        uint vertex, fragment;
        int success;
        char infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexCodeStr, nullptr);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            LOG_ERROR("Vertex shader compilation failed.\n{}", infoLog);
        };

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentCodeStr, nullptr);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            LOG_ERROR("Fragment shader compilation failed.\n{}", infoLog);
        };

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, nullptr, infoLog);
            LOG_ERROR("Shader program linking failed.\n{}", infoLog);
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::use() const
    {
        glUseProgram(ID);
    }

    void Shader::setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }

    void Shader::setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setVec3f(const std::string& name, const float* value_ptr) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value_ptr);
    }

    void Shader::setMat4fv(const std::string& name, GLboolean transpose, const float* value_ptr) const
    {
        uint loc = glGetUniformLocation(ID, name.c_str());
        glUniformMatrix4fv(loc, 1, transpose, value_ptr);
    }

    void Shader::setMat3fv(const std::string& name, int transpose, const float* value_ptr) const
    {
        uint loc = glGetUniformLocation(ID, name.c_str());
        glUniformMatrix3fv(loc, 1, transpose, value_ptr);
    }

    void Shader::setPointLight(const std::string& name, const CPointLight& options) const
    {
        setFloat(name + ".constant", options.constant);
        setFloat(name + ".linear", options.linear);
        setFloat(name + ".quadratic", options.quadratic);
        setVec3f(name + ".position", glm::value_ptr(options.position));
        setVec3f(name + ".ambient", glm::value_ptr(options.ambient));
        setVec3f(name + ".diffuse", glm::value_ptr(options.diffuse));
        setVec3f(name + ".specular", glm::value_ptr(options.specular));
    }

    void Shader::setSpotlight(const std::string& name, const CSpotlight& options) const
    {
        setFloat(name + ".constant", options.constant);
        setFloat(name + ".linear", options.linear);
        setFloat(name + ".quadratic", options.quadratic);
        setFloat(name + ".innerCutOff", options.innerCutOff);
        setFloat(name + ".outerCutOff", options.outerCutOff);

        setVec3f(name + ".direction", glm::value_ptr(options.direction));
        setVec3f(name + ".position", glm::value_ptr(options.position));
        setVec3f(name + ".ambient", glm::value_ptr(options.ambient));
        setVec3f(name + ".diffuse", glm::value_ptr(options.diffuse));
        setVec3f(name + ".specular", glm::value_ptr(options.specular));
    }

    void Shader::setDirectionalLight(const std::string& name, const CDirectionalLight& options) const
    {
        setVec3f(name + ".direction", glm::value_ptr(options.direction));
        setVec3f(name + ".ambient", glm::value_ptr(options.ambient));
        setVec3f(name + ".diffuse", glm::value_ptr(options.diffuse));
        setVec3f(name + ".specular", glm::value_ptr(options.specular));
    }

    void Shader::setPointLightArray(const std::string& name, const std::vector<CPointLight>& lights) const
    {
        unsigned int i = 0;
        for (const CPointLight& light : lights)
        {
            setPointLight(name + "[" + std::to_string(i) + "]", light);
            i++;
        }
    }

    void Shader::setSpotlightArray(const std::string& name, const std::vector<CSpotlight>& lights) const
    {
        unsigned int i = 0;
        for (const CSpotlight& light : lights)
        {
            setSpotlight(name + "[" + std::to_string(i) + "]", light);
        }
    }

    void Shader::setDirLightArray(const std::string& name, const std::vector<CDirectionalLight>& lights) const
    {
        unsigned int i = 0;
        for (const CDirectionalLight& light : lights)
        {
            setDirectionalLight(name + "[" + std::to_string(i) + "]", light);
        }
    }
}