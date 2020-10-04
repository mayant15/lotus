#pragma once

namespace Lotus
{
    // TODO: Should I split "systems" into two parts? One with static callbacks and the other with the actual data.
    // So the "InputSystem" would be callbacks for input events, but state will be maintained in "InputSystemData",
    // that users will be able to query.

    class ISystem // : public ILifecycle
    {
        // Register a set of callbacks here

        // EVENT()
        // void OnCustomEvent(const CustomEvent& event, Args&& args...)
    };
}
