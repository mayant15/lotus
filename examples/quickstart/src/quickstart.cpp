#include <lotus/lotus.h>
#include <lotus/debug.h>
#include <quickstart_export.h>

#define BALL_TAG "ball"

struct CTag
{
    std::string Tag;
    GENERATED_BODY(CTag, Tag);
};

void OnContact(const Lotus::CollisionEvent& event)
{
    LOG_INFO("Collision!");
}

//=============================================================================
// REGISTRATION FUNCTIONS
//=============================================================================

QUICKSTART_API void ModuleTestFunction()
{
    LOG_INFO("Loading [module:quickstart]");
}

QUICKSTART_API void RegisterEvents()
{
    using namespace Lotus;

    LOG_INFO("Registering [module:quickstart]");

    EventManager& em = GET(EventManager);
    em.Bind<CollisionEvent, OnContact>();

    RegisterComponent<CTag>();
}
