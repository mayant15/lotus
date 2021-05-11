#include <lotus/resources/Shader.h>
#include <lotus/filesystem.h>
#include "rendering/opengl/GLShader.h"

namespace Lotus
{
    SRef<Shader> ShaderLoader::Load(const std::string& relVertexPath, const std::string& relFragmentPath) const
    {
        // TODO: Get the render API from a config class?
        return std::make_shared<GLShader>(ExpandPath(relVertexPath), ExpandPath(relFragmentPath));
    }
}
