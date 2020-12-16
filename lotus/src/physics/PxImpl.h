#pragma once

// Unix needs this defined and wouldn't take CMake's, idk why
#if defined(__unix__)
#define _DEBUG
#endif

// The include files for PhysX are no longer installed in the correct vcpkg directory,
// they're copied right there in the include path.
#include <PxPhysicsAPI.h>

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