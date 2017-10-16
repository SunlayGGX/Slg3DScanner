#include "GamepadController.h"

#include "DirectXUtilitary.h"

using namespace Slg3DScanner;


GamepadController::InternalGamepadControllerElement::InternalGamepadControllerElement() :
    m_back{ &m_gamepadFirst },
    m_front{ &m_gamepadSecond },
    m_previous{ 0 }
{

}

void GamepadController::InternalGamepadControllerElement::swapFrontBack()
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    std::swap(m_back, m_front);
}

bool GamepadController::InternalGamepadControllerElement::update()
{
    return m_back->update(m_previous);
}

void GamepadController::swapFrontBack()
{
    m_firstGamepad.swapFrontBack();
}

uint8_t GamepadController::update()
{
    m_firstGamepad.swapFrontBack();

    uint8_t result = 0;
    result |= static_cast<uint8_t>(m_firstGamepad.update());

    return result;
}

Gamepad GamepadController::getFirstGamepad() const
{
    std::lock_guard<std::mutex> autoLocker{ m_firstGamepad.m_mutex };
    return *m_firstGamepad.m_front;
}