#include "Gamepad.h"

#include "DirectXUtilitary.h"

using namespace Slg3DScanner;


Gamepad::Gamepad() :
    m_buttonsChangedThisFrame{ 0 },
    m_buttonsDownThisFrame{ 0 },
    m_buttonsUpThisFrame{ 0 }
{
    DXZeroMemory(m_currentState);
}

Gamepad::~Gamepad() = default;

void Gamepad::internalUpdateKeys(const Buttons& previousGamepadState)
{
    m_buttonsChangedThisFrame = m_currentState.Gamepad.wButtons ^ previousGamepadState;
    m_buttonsDownThisFrame = m_buttonsChangedThisFrame & m_currentState.Gamepad.wButtons;
    m_buttonsUpThisFrame = m_buttonsChangedThisFrame & (~m_currentState.Gamepad.wButtons);
}

bool Gamepad::update(Buttons& previousGamepadState)
{
    if(XInputGetState(0, &m_currentState) == ERROR_SUCCESS)
    {
        this->internalUpdateKeys(previousGamepadState);

        previousGamepadState = m_currentState.Gamepad.wButtons;

        return true;
    }

    return false;
}
