#include "KeyboardController.h"

#include "DirectXUtilitary.h"

using namespace Slg3DScanner;


KeyboardController::InternalKeyboardControllerElement::InternalKeyboardControllerElement() :
    m_back{ &m_keyboardFirst },
    m_front{ &m_keyboardSecond },
    m_inputDevice{ nullptr }
{

}

void KeyboardController::InternalKeyboardControllerElement::destroy()
{
    DXUnacquire(m_inputDevice);
    DXRelease(m_inputDevice);
}

KeyboardController::KeyboardController() :
    m_directInputControl{ nullptr }
{

}

KeyboardController::~KeyboardController()
{
    m_keyboard.destroy();
    DXRelease(m_directInputControl);
}

void KeyboardController::InternalKeyboardControllerElement::swapFrontBack()
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    std::swap(m_back, m_front);
}

bool KeyboardController::InternalKeyboardControllerElement::update()
{
    m_back->update(m_inputDevice, m_keyboardBuffer, m_previous);

    return true;
}

void KeyboardController::swapFrontBack()
{
    m_keyboard.swapFrontBack();
}

bool KeyboardController::update()
{
    m_keyboard.swapFrontBack();

    return m_keyboard.update();
}

KeyboardElement KeyboardController::getCurrentKeyboardState() const
{
    std::lock_guard<std::mutex> autoLocker{ m_keyboard.m_mutex };
    return m_keyboard.m_front->getCurrentState();
}

KeyboardElement KeyboardController::getKeyboardChangedState() const
{
    std::lock_guard<std::mutex> autoLocker{ m_keyboard.m_mutex };
    return m_keyboard.m_front->getKeyChangedStateThisFrame();
}

KeyboardElement KeyboardController::getKeyboardUpState() const
{
    std::lock_guard<std::mutex> autoLocker{ m_keyboard.m_mutex };
    return m_keyboard.m_front->getKeyUpThisFrame();
}

KeyboardElement KeyboardController::getKeyboardDownState() const
{
    std::lock_guard<std::mutex> autoLocker{ m_keyboard.m_mutex };
    return m_keyboard.m_front->getKeyDownThisFrame();
}

Keyboard KeyboardController::getCurrentKeyboard() const
{
    std::lock_guard<std::mutex> autoLocker{ m_keyboard.m_mutex };
    return *m_keyboard.m_front;
}

void KeyboardController::createKeyboard(HWND windowsInstance)
{
    if(windowsInstance != nullptr)
    {
        m_keyboard.destroy();
        DXRelease(m_directInputControl);

        DXTry(DirectInput8Create(
            GetModuleHandle(nullptr),
            DIRECTINPUT_VERSION,
            IID_IDirectInput8,
            reinterpret_cast<void**>(&m_directInputControl),
            nullptr
        ));

        DXTry(m_directInputControl->CreateDevice(GUID_SysKeyboard,
            &m_keyboard.m_inputDevice,
            __nullptr)
        );

        DXTry(m_keyboard.m_inputDevice->SetDataFormat(&c_dfDIKeyboard));

        m_keyboard.m_inputDevice->SetCooperativeLevel(
            windowsInstance,
            DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
        );

        DXAcquire(m_keyboard.m_inputDevice);
    }
}
