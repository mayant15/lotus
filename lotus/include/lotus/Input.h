#pragma once

#include "lotus/lcommon.h"
#include "lotus/ecs/Event.h"

/**
 * Input manager that receives window events and maintains input state. Users should query this class
 * to get input information. This is to avoid end users interacting with low level window implementations
 * themselves.
 */
namespace Lotus::Input
{
    /**
     * KeyboardEvent receiver that updates internal keyboard state.
     * @param event Keyboard input data
     */
    LOTUS_API void UpdateKeyState(const KeyboardEvent& event);

    /**
     * MouseEvent receiver that updates internal mouse state.
     * @param event Mouse input data
     */
    LOTUS_API void UpdateMouseState(const MouseEvent& event);

    /**
     * Query the keyboard state for a particular key
     * @param key Keycode for the requested key
     * @return True if the key is pressed
     */
    LOTUS_API bool GetKeyPressed(int key);

    /**
     * Query the mouse state for mouse cursor delta since the last mouse event
     * @return Pair of floats with X and Y deltas
     */
    LOTUS_API std::pair<float, float> GetMouseDelta();
}
