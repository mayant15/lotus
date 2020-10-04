#include "GLShader.h"

#include "glad/glad.h"
#include "lotus/debug.h"

#include <fstream>

namespace Lotus
{
    GLShader::GLShader(const std::string& vertexPath, const std::string& fragmentPath)
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

        unsigned int vertex, fragment;
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
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentCodeStr, nullptr);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            LOG_ERROR("Fragment shader compilation failed.\n{}", infoLog);
        }

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

    void GLShader::Use() const
    {
        glUseProgram(ID);
    }

    void GLShader::SetInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void GLShader::SetFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void GLShader::SetBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void GLShader::SetMat4f(const std::string& name, const Matrix4f& mat) const
    {
        const int loc = glGetUniformLocation(ID, name.c_str());
        glUniformMatrix4fv(loc, 1, false, valuePtr(mat));
    }

    void GLShader::SetVec3f(const std::string& name, const Vector3f& vec) const
    {
        const int loc = glGetUniformLocation(ID, name.c_str());
        glUniform3fv(loc, 1, valuePtr(vec));
    }

    void GLShader::SetPointLight(const std::string& name, const PointLightInfo& options) const
    {
        SetFloat(name + ".constant", options.constant);
        SetFloat(name + ".linear", options.linear);
        SetFloat(name + ".quadratic", options.quadratic);
        SetVec3f(name + ".position", options.position);
        SetVec3f(name + ".ambient", options.ambient);
        SetVec3f(name + ".diffuse", options.diffuse);
        SetVec3f(name + ".specular", options.specular);
    }

    void GLShader::SetSpotlight(const std::string& name, const SpotLightInfo& options) const
    {
        SetFloat(name + ".constant", options.constant);
        SetFloat(name + ".linear", options.linear);
        SetFloat(name + ".quadratic", options.quadratic);
        SetFloat(name + ".innerCutOff", options.innerCutOff);
        SetFloat(name + ".outerCutOff", options.outerCutOff);

        SetVec3f(name + ".direction", options.direction);
        SetVec3f(name + ".position", options.position);
        SetVec3f(name + ".ambient", options.ambient);
        SetVec3f(name + ".diffuse", options.diffuse);
        SetVec3f(name + ".specular", options.specular);
    }

    void GLShader::SetDirectionalLight(const std::string& name, const LightInfo& options) const
    {
        SetVec3f(name + ".direction", options.direction);
        SetVec3f(name + ".ambient", options.ambient);
        SetVec3f(name + ".diffuse", options.diffuse);
        SetVec3f(name + ".specular", options.specular);
    }

    void GLShader::SetPointLightArray(const std::string& name, const std::vector<PointLightInfo>& lights) const
    {
        unsigned int i = 0;
        for (const PointLightInfo& light : lights)
        {
            SetPointLight(name + "[" + std::to_string(i) + "]", light);
            i++;
        }
    }

    void GLShader::SetSpotlightArray(const std::string& name, const std::vector<SpotLightInfo>& lights) const
    {
        unsigned int i = 0;
        for (const SpotLightInfo& light : lights)
        {
            SetSpotlight(name + "[" + std::to_string(i) + "]", light);
        }
    }

    void GLShader::SetDirLightArray(const std::string& name, const std::vector<LightInfo>& lights) const
    {
        unsigned int i = 0;
        for (const LightInfo& light : lights)
        {
            SetDirectionalLight(name + "[" + std::to_string(i) + "]", light);
        }
    }

    void GLShader::SetMaterial(const std::string& name, Handle<Material> mat) const
    {
        // Set albedo
        auto albedo = mat->Albedo;
        if (std::holds_alternative<Vector3f>(albedo))
        {
            // invalid texture, set the color
            SetVec3f(name + ".vAlbedo", std::get<Vector3f>(albedo));
            SetBool(name + ".bUseAlbedoTex", false);
        }
        else
        {
            // valid texture, set texture
            SetBool(name + ".bUseAlbedoTex", true);
        
            Handle<Texture> texture = std::get<Handle<Texture>>(albedo);
            glActiveTexture(GL_TEXTURE1);
            SetInt(name + ".tAlbedo", 1);
            glBindTexture(GL_TEXTURE_2D, texture->ID);
        }

        // Set normal map
        auto normal = mat->Normal;
        if (normal)
        {
            SetBool(name + ".bUseNormalTex", true);

            glActiveTexture(GL_TEXTURE2);
            SetInt(name + ".tNormal", 2);
            glBindTexture(GL_TEXTURE_2D, normal.value()->ID);
        }
        else
        {
            SetBool(name + ".bUseNormalTex", false);
        }

        SetFloat(name + ".fRoughness", mat->Roughness);
        SetFloat(name + ".fMetallic", mat->Metallic);
        SetFloat(name + ".fAO", mat->AO);
    }
}
