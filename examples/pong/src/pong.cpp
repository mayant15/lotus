#include <lotus/lotus.h>
#include <lotus/debug.h>
#include <pong_export.h>

struct CPaddle
{
    int ID = 0;
    GENERATED_BODY(CPaddle, ID);
};

//=============================================================================
// REGISTRATION FUNCTIONS
//=============================================================================

PONG_API void ModuleTestFunction()
{
    LOG_INFO("Loading [module:pong]");
}

PONG_API void RegisterEvents()
{
    using namespace Lotus;
    LOG_INFO("Registering [module:pong]");

    RegisterComponent<CPaddle>();
}
