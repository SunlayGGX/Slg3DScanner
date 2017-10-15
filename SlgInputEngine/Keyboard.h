#pragma once

#include "KeyboardElement.h"

namespace Slg3DScanner
{
    class Keyboard
    {
    public:
        enum
        {
            KEYBOARD_COUNT = 256
        };


    private:
        KeyboardElement m_current;

        KeyboardElement m_changedThisFrame;
        KeyboardElement m_keyDownThisFrame;
        KeyboardElement m_keyUpThisFrame;


    public:
        Keyboard();
        ~Keyboard();

        void update(IDirectInputDevice8* inputDevice, char(&buffer)[Keyboard::KEYBOARD_COUNT], KeyboardElement& previousState);
        void readKeyboard(IDirectInputDevice8* inputDevice, char(&buffer)[Keyboard::KEYBOARD_COUNT]);

        const KeyboardElement& getCurrentState() const;

        const KeyboardElement& getKeyChangedStateThisFrame() const;
        const KeyboardElement& getKeyDownThisFrame() const;
        const KeyboardElement& getKeyUpThisFrame() const;


    private:
        void internalUpdateKeys(const KeyboardElement& previousState);
        bool internalReadKeyboard(IDirectInputDevice8* inputDevice, char(*temporaryBuffer)[Keyboard::KEYBOARD_COUNT]);
    };
}
