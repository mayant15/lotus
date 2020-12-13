#pragma once

// Somehow the include files for PhysX are not installed in the correct vcpkg directory,
// they're copied right there in the include path
#if defined(__unix__)
#define _DEBUG
#include <PxPhysicsAPI.h>
#else
#include <physx/PxPhysicsAPI.h>
#endif

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