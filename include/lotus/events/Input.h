#pragma once

#include "lotus/lcommon.h"
#include "Event.h"

#include <unordered_map>

namespace Lotus
{
    class Input : public Singleton<Input>
    {
        std::unordered_map<int, int> _state;

        float _xOffset = 0.0f;
        float _yOffset = 0.0f;

    private:
        friend Singleton<Input>;

        Input() = default;

    public:
        void UpdateKeyState(const KeyboardEvent& event);

        void UpdateMouseState(const MouseEvent& event);

        bool GetKeyPressed(int key);

        std::pair<float, float> GetMouseDelta();
    };
}
