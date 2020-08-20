#pragma once

#include "lotus/lcommon.h"
#include "lotus/resources/Model.h"
#include "lotus/resources/Cubemap.h"
#include "lotus/resources/Shader.h"

namespace Lotus
{
    struct CMeshRenderer
    {
        // Render data
        Handle<Shader> Shader;
        Handle<Model> Model;
    };

    struct CSkybox
    {
       Handle<Shader> Shader;
       Handle<Cubemap> Map;
    };
}
