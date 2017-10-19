#include "InputEngine.h"

#include "GamepadController.h"
#include "KeyboardController.h"

#include "LoggerCommand.h"

using namespace Slg3DScanner;

InputEngine::InputEngine() :
    m_gamepadController{ std::make_unique<Slg3DScanner::GamepadController>() },
    m_keyboardController{ std::make_unique<Slg3DScanner::KeyboardController>() }
{
}

InputEngine::~InputEngine()
{

}

void InputEngine::initialize()
{
    
}

void InputEngine::destroy()
{

}

void InputEngine::update()
{
    if(!m_gamepadController->update())
    {

    }

    m_keyboardController->update();
}

Gamepad InputEngine::getCurrentGamepadState() const
{
    return m_gamepadController->getFirstGamepad();
}

void InputEngine::createKeyboard(HWND windowsInstance)
{
    m_keyboardController->createKeyboard(windowsInstance);
}

KeyboardElement InputEngine::getCurrentKeyboardState() const
{
    return m_keyboardController->getCurrentKeyboardState();
}

KeyboardElement InputEngine::getKeyboardChangedState() const
{
    return m_keyboardController->getKeyboardChangedState();
}

KeyboardElement InputEngine::getKeyboardUpState() const
{
    return m_keyboardController->getKeyboardUpState();
}

KeyboardElement InputEngine::getKeyboardDownState() const
{
    return m_keyboardController->getKeyboardDownState();
}
