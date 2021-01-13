#pragma once

#include <lotus/lotus_export.h>

namespace Lotus::Engine
{
    /** @brief Initializes the engine and various subsystems */
    LOTUS_API void Initialize();

    /** @brief Logic for a single engine tick */
    LOTUS_API void Tick(double deltaTime);

    /** @brief Cleanup and shutdown */
    LOTUS_API void Shutdown();
}
