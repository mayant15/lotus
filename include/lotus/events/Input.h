#pragma once

#include "Event.h"

namespace Lotus
{
    class Input : public Singleton<Input>
    {
        std::unordered_map<int, int> _state;
    private:
        friend Singleton<Input>;

        Input() = default;

    public:
        void UpdateState(const KeyboardEvent& event)
        {
            try
            {
                _state.at(event.KeyCode) = event.State;
            }
            catch (const std::exception& e)
            {
                // TODO: Error if exception is legit, and not just a "no key found"
                _state.insert({event.KeyCode, event.State});
            }
        }

        bool GetKeyPressed(int key)
        {
            try
            {
                int state = _state.at(key);
                return (state == L_KEY_PRESS || state == L_KEY_REPEAT);
            }
            catch (const std::exception& e)
            {
                // TODO: Error if exception is legit, and not just a "no key found"
                return false;
            }
        }
    };
}
