#pragma once

#include "lotus/lcommon.h"
#include "lotus/resources/Model.h"
#include "lotus/resources/Cubemap.h"

namespace Lotus
{
    class LShader;
    struct CMeshRenderer
    {
        // Render data
        SRef<LShader> Shader;
        Handle<Model> Model;
    };

    struct CSkybox
    {
       SRef<LShader> Shader;
       Handle<Cubemap> Map;
    };
}
