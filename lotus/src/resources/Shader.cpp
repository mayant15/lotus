#include "lotus/resources/Shader.h"
#include "GLShader.h"

namespace Lotus
{
    SRef<Shader> ShaderLoader::Load(const std::string& vertexPath, const std::string& fragmentPath) const
    {
        // TODO: Get the render API from a config class?
        return std::make_shared<GLShader>(vertexPath, fragmentPath);
    }
}
