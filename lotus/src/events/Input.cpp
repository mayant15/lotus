#include "lotus/events/Input.h"

namespace Lotus
{
    static float lastX = 0.0f;
    static float lastY = 0.0f;
    static bool firstMouse = true;

    void Input::UpdateKeyState(const KeyboardEvent& event)
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

    void Input::UpdateMouseState(const MouseEvent& event)
    {
        Input& input = Input::Get();
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

    std::pair<float, float> Input::GetMouseDelta()
    {
        return {_xOffset, _yOffset};
    }
}
