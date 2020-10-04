#pragma once

#include <physx/PxPhysicsAPI.h>

namespace Lotus::Physics
{
    using namespace physx;

    /**
     * @brief Implements logging callbacks used by PhysX
    */
    class PhysXErrorCallback : public PxErrorCallback
    {
        /**
         * @brief Callback that logs PhysX error messages
         * @param code Error code
         * @param message Message string
         * @param file Source filename
         * @param line Line number in source file
        */
        void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) override;
    };
}