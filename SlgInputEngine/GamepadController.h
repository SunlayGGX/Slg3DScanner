#pragma once

#include "Gamepad.h"

namespace Slg3DScanner
{
    class GamepadController
    {
    private:
        struct InternalGamepadControllerElement
        {
        public:
            Gamepad m_gamepadFirst;
            Gamepad m_gamepadSecond;

            Gamepad* m_front;
            Gamepad* m_back;

            Gamepad::Buttons m_previous;

            mutable std::mutex m_mutex;


        public:
            InternalGamepadControllerElement();

            void swapFrontBack();
            bool update();
        };


    private:
        InternalGamepadControllerElement m_firstGamepad;


    public:
        GamepadController() = default;
        ~GamepadController() = default;

        uint8_t update();

        Gamepad getFirstGamepad() const;


    private:
        void swapFrontBack();
    };
}
