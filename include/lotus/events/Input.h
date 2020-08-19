#pragma once

#include "lotus/lcommon.h"
#include "Event.h"

#include <unordered_map>

namespace Lotus
{
    /**
     * Input manager that receives window events and maintains input state. Users should query this class
     * to get input information. This is to avoid end users interacting with low level window implementations
     * themselves.
     */
    class LOTUS_API Input : public Singleton<Input>
    {
        std::unordered_map<int, int> _state;

        float _xOffset = 0.0f;
        float _yOffset = 0.0f;

    public:

        /**
         * KeyboardEvent receiver that updates internal keyboard state.
         * @param event Keyboard input data
         */
        void UpdateKeyState(const KeyboardEvent& event);

        /**
         * MouseEvent receiver that updates internal mouse state.
         * @param event Mouse input data
         */
        void UpdateMouseState(const MouseEvent& event);

        /**
         * Query the keyboard state for a particular key
         * @param key Keycode for the requested key
         * @return True if the key is pressed
         */
        bool GetKeyPressed(int key);

        /**
         * Query the mouse state for mouse cursor delta since the last mouse event
         * @return Pair of floats with X and Y deltas
         */
        std::pair<float, float> GetMouseDelta();

    private:
        friend Singleton<Input>;

        Input() = default;
    };
}
