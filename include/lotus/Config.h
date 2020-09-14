#pragma once

#include "lotus/lcommon.h"

namespace Lotus
{
    /**
     * @brief Singleton class that holds all configuration options for the engine.
    */
    class Config : public Singleton<Config>
    {
        Config() = default;
        friend Singleton<Config>;
    public:
        ERenderAPI RenderAPI = ERenderAPI::OPEN_GL;
        bool IsDebug = true;
        unsigned int Width = 800;
        unsigned int Height = 600;
        std::string ResourceRoot = "";
    };
}
