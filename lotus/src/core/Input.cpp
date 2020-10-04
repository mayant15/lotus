#include "lotus/Input.h"

namespace Lotus
{
    static float lastX = 0.0f;
    static float lastY = 0.0f;
    static bool firstMouse = true;

    void Input::UpdateKeyState(const KeyboardEvent& event)
    {
        if (_state.find(event.KeyCode) != _state.end())
        {
            _state.at(event.KeyCode) = event.State;
        }
        else
        {
            _state.insert({event.KeyCode, event.State});
        }
    }

    void Input::UpdateMouseState(const MouseEvent& event)
    {
        if (firstMouse)
        {
            lastX = event.MouseX;
            lastY = event.MouseY;
            firstMouse = false;
        }

        _xOffset = event.MouseX - lastX;
        _yOffset = lastY - event.MouseY; // reversed since y-coordinates go from bottom to top

        lastX = event.MouseX;
        lastY = event.MouseY;
    }

    bool Input::GetKeyPressed(int key)
    {
        if (_state.find(key) != _state.end())
        {
            int state = _state.at(key);
            return (state == L_KEY_PRESS || state == L_KEY_REPEAT);
        }
        else
        {
            return false;
        }
    }

    std::pair<float, float> Input::GetMouseDelta()
    {
        return {_xOffset, _yOffset};
    }
}
