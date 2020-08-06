#pragma once

#include "lotus/lcommon.h"

namespace Lotus
{
    class LShader;
    class LModel;
    struct CMeshRenderer
    {
        // Render data
        SRef<LShader> Shader;
        SRef<LModel> Model;
    };
}
