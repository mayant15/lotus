#pragma once

#include "lotus/lcommon.h"

namespace Lotus
{
    /**
     * @brief Singleton class that holds all configuration options for the engine.
    */
    class Config : public Singleton<Config>
    {
    public:
        // TODO: Pick this up from an INI file
        // TODO: Should this be a singleton?
        ERenderAPI RenderAPI = ERenderAPI::OPEN_GL;
        bool IsDebug = true;
        unsigned int Width = 800;
        unsigned int Height = 600;
        std::string ResourceRoot = "";

    private:
        Config() = default;
        friend Singleton<Config>;
    };
}
