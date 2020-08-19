#pragma once

#include "lotus/lcommon.h"

namespace Lotus
{
    class LShader;
    class LModel;
    class Cubemap;
    struct LOTUS_API CMeshRenderer
    {
        // Render data
        SRef<LShader> Shader;
        SRef<LModel> Model;
    };

    struct CSkybox
    {
       SRef<LShader> Shader;
       SRef<Cubemap> Map;
    };
}
