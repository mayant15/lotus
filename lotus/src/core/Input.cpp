#include "lotus/Input.h"

#include <unordered_map>

namespace Lotus::Input
{
    double lastX = 0.0f;
    double lastY = 0.0f;
    double xOffset = 0.0f;
    double yOffset = 0.0f;

    bool firstMouse = true;

    std::unordered_map<int, int> keyStateMap = {};
    bool mouseButtonPressed[MOUSE_BUTTON_COUNT] = {false, false, false};

    void UpdateKeyState(int keycode, int state)
    {
        if (keyStateMap.find(keycode) != keyStateMap.end())
        {
            keyStateMap.at(keycode) = state;
        }
        else
        {
            keyStateMap.insert({keycode, state});
        }
    }

    void UpdateMouseButtonState(bool left, bool right, bool middle)
    {
        mouseButtonPressed[L_MOUSE_LEFT] = left;
        mouseButtonPressed[L_MOUSE_RIGHT] = right;
        mouseButtonPressed[L_MOUSE_MIDDLE] = middle;
    }

    void UpdateMouseState(double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        xOffset = xpos - lastX;
        yOffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;
    }

    bool GetKeyPressed(int key)
    {
        if (keyStateMap.find(key) != keyStateMap.end())
        {
            int status = keyStateMap.at(key);
            return (status == L_KEY_PRESS || status == L_KEY_REPEAT);
        }
        else
        {
            return false;
        }
    }

    std::pair<double, double> GetMouseDelta()
    {
        return {xOffset, yOffset};
    }
}
