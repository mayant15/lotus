/**
 * Use this file to implement callbacks necessary for working with PhysX
 */

#pragma once

#include "physx/PxPhysicsAPI.h"

namespace Lotus::Physics
{
    using namespace physx;

    class PhysXErrorCallback : public PxErrorCallback
    {
        void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) override;
    };
}