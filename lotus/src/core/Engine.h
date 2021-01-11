#pragma once

#include <lotus/lcommon.h>
#include <lotus/ecs/Event.h>

namespace Lotus::Engine
{
    /** @brief Initializes the engine and various subsystems */
    void Initialize();

    /** @brief Start engine ticks */
    void Run();

    /** @brief Cleanup and shutdown */
    void Shutdown();

    /** @brief Data that maintains state for the Engine system */
    struct State
    {
        /** @brief True if engine is ticking */
        bool IsRunning = true;
    };
}
