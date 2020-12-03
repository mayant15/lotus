#include "lotus/Input.h"

#include <unordered_map>

namespace Lotus
{
    float lastX = 0.0f;
    float lastY = 0.0f;
    float xOffset = 0.0f;
    float yOffset = 0.0f;

    bool firstMouse = true;

    std::unordered_map<int, int> state = {};

    void Input::UpdateKeyState(const KeyboardEvent& event)
    {
        if (state.find(event.KeyCode) != state.end())
        {
            state.at(event.KeyCode) = event.State;
        }
        else
        {
            state.insert({event.KeyCode, event.State});
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

        xOffset = event.MouseX - lastX;
        yOffset = lastY - event.MouseY; // reversed since y-coordinates go from bottom to top

        lastX = event.MouseX;
        lastY = event.MouseY;
    }

    bool Input::GetKeyPressed(int key)
    {
        if (state.find(key) != state.end())
        {
            int status = state.at(key);
            return (status == L_KEY_PRESS || status == L_KEY_REPEAT);
        }
        else
        {
            return false;
        }
    }

    std::pair<float, float> Input::GetMouseDelta()
    {
        return {xOffset, yOffset};
    }
}
