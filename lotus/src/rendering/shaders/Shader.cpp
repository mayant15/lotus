#include <fstream>
#include <iostream>
#include <string>

#include "lotus/rendering/Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
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
        std::cout << "ERROR::SHADER::PROGRAM::FILE_NOT_READ\n" << std::endl;
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
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCodeStr, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
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
