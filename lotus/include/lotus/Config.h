#pragma once

#include <lotus/lcommon.h>

namespace Lotus
{
    /**
     * @brief Class that holds all configuration options for the engine.
    */
    class Config
    {
    public:
        // TODO: Pick this up from a config file
        ERenderAPI RenderAPI = ERenderAPI::OPEN_GL;
        bool IsDebug = true;
        unsigned int Width = 800;
        unsigned int Height = 600;
    };
}
