#pragma once

#include "Keyboard.h"

namespace Slg3DScanner
{
    class KeyboardController
    {
    private:
        struct InternalKeyboardControllerElement
        {
        public:
            char m_keyboardBuffer[Keyboard::KEYBOARD_COUNT];

            Keyboard m_keyboardFirst;
            Keyboard m_keyboardSecond;

            Keyboard* m_front;
            Keyboard* m_back;

            KeyboardElement m_previous;

            mutable std::mutex m_mutex;

            IDirectInputDevice8* m_inputDevice;


        public:
            InternalKeyboardControllerElement();

            bool update();
            void swapFrontBack();

            void destroy();
        };


    private:
        InternalKeyboardControllerElement m_keyboard;
        IDirectInput8* m_directInputControl;


    public:
        KeyboardController();
        ~KeyboardController();

        bool update();

        Keyboard getCurrentKeyboard() const;

        KeyboardElement getCurrentKeyboardState() const;
        KeyboardElement getKeyboardChangedState() const;
        KeyboardElement getKeyboardUpState() const;
        KeyboardElement getKeyboardDownState() const;

        void createKeyboard(HWND windowsInstance);


    private:
        void swapFrontBack();
    };
}
