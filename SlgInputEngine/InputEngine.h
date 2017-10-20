#pragma once

#include "SlgSingleton.h"

#include "KeyboardElement.h"
#include "Gamepad.h"


namespace Slg3DScanner
{
    class InputEngine : public Slg3DScanner::SlgSingleton<InputEngine>
    {
    private:
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(InputEngine);


    private:
        std::unique_ptr<class GamepadController> m_gamepadController;
        std::unique_ptr<class KeyboardController> m_keyboardController;


    public:
        void update();

        void createKeyboard(HWND windowsInstance);

        Gamepad getCurrentGamepadState() const;

        KeyboardElement getCurrentKeyboardState() const;
        KeyboardElement getKeyboardChangedState() const;
        KeyboardElement getKeyboardUpState() const;
        KeyboardElement getKeyboardDownState() const;
    };
}
