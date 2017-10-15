#include "InputEngine.h"

#include "GamepadController.h"
#include "KeyboardController.h"

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
        //TODO: Log
    }

    m_keyboardController->update();
}

void InputEngine::createKeyboard(HWND windowsInstance)
{
    m_keyboardController->createKeyboard(windowsInstance);
}
