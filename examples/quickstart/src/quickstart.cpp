#include "GameCamera.h"
#include <lotus/ecs/EventManager.h>
#include <lotus/debug.h>

void OnCollision(const Lotus::CollisionEvent& event)
{
    LOG_INFO("Contact");
}

// TODO: Call this function when play is pressed?
void SetupGameEvents()
{
    using namespace Lotus;
    EventManager& em = GET(EventManager);
    em.Bind<UpdateEvent, GameCamera::OnUpdate>();
    em.Bind<MouseEvent, GameCamera::OnMouseEvent>();
    em.Bind<CollisionEvent, OnCollision>();
}
