#include "PxImpl.h"
#include "lotus/debug.h"

namespace Lotus::Physics
{
    void PhysXErrorCallback::reportError(PxErrorCode::Enum code, const char* message, const char* file, int line)
    {
        const char* errorCode = nullptr;

        switch (code)
        {
        case PxErrorCode::eNO_ERROR:
            errorCode = "no error";
            break;
        case PxErrorCode::eINVALID_PARAMETER:
            errorCode = "invalid parameter";
            break;
        case PxErrorCode::eINVALID_OPERATION:
            errorCode = "invalid operation";
            break;
        case PxErrorCode::eOUT_OF_MEMORY:
            errorCode = "out of memory";
            break;
        case PxErrorCode::eDEBUG_INFO:
            errorCode = "info";
            break;
        case PxErrorCode::eDEBUG_WARNING:
            errorCode = "warning";
            break;
        case PxErrorCode::ePERF_WARNING:
            errorCode = "performance warning";
            break;
        case PxErrorCode::eABORT:
            errorCode = "abort";
            break;
        case PxErrorCode::eINTERNAL_ERROR:
            errorCode = "internal error";
            break;
        case PxErrorCode::eMASK_ALL:
            errorCode = "unknown error";
            break;
        }

        if (errorCode)
        {
            LOG_WARN("{} ({}): {} - {}", file, line, errorCode, message);
        }
    }
}