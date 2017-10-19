#include "Keyboard.h"

#include "DirectXUtilitary.h"

using namespace Slg3DScanner;

bool KeyboardElement::getKey(const unsigned char key) const
{
    return (m_array[key / 64] >> (key % 64)) & 0x1;
}

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{

}

void Keyboard::internalUpdateKeys(const KeyboardElement& previousState)
{
    for(auto iter = 0; iter != 4; ++iter)
    {
        m_changedThisFrame.m_array[iter]  = m_current.m_array[iter] ^ previousState.m_array[iter];
        m_keyDownThisFrame.m_array[iter] = m_changedThisFrame.m_array[iter] & m_current.m_array[iter];
        m_keyUpThisFrame.m_array[iter] = m_changedThisFrame.m_array[iter] & (~m_current.m_array[iter]);
    }
}

bool Keyboard::internalReadKeyboard(IDirectInputDevice8* inputDevice, char(*temporaryBuffer)[Keyboard::KEYBOARD_COUNT])
{
    switch(inputDevice->GetDeviceState(sizeof(*temporaryBuffer), static_cast<void*>(*temporaryBuffer)))
    {
    case S_OK:
        return true;

    case DIERR_NOTACQUIRED:
    case DIERR_INPUTLOST:
        if(inputDevice->Acquire() == S_OK)
        {
            return inputDevice->GetDeviceState(sizeof(temporaryBuffer), static_cast<void*>(&temporaryBuffer)) == S_OK;
        }
        return false;

    default:
        return false;
    }
}

void Keyboard::readKeyboard(IDirectInputDevice8* inputDevice, char(&buffer)[Keyboard::KEYBOARD_COUNT])
{
    DXZeroMemory(m_current);

    if(this->internalReadKeyboard(inputDevice, &buffer))
    {
        int64_t* current = m_current.m_array;
        unsigned int iter = 0;
        for(; iter != 64;)
        {
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
        }

        ++current;
        for(; iter != 128;)
        {
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
        }

        ++current;
        for(; iter != 192;)
        {
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
        }

        ++current;
        for(; iter != 256;)
        {
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
            *current |= (static_cast<int64_t>((buffer[iter] & 0x80) >> 7) << iter); ++iter;
        }
    }
}

void Keyboard::update(IDirectInputDevice8* inputDevice, char(&buffer)[Keyboard::KEYBOARD_COUNT], KeyboardElement& previousState)
{
    this->readKeyboard(inputDevice, buffer);
    
    this->internalUpdateKeys(previousState);

    previousState = m_current;
}

const KeyboardElement& Keyboard::getCurrentState() const
{
    return m_current;
}

const KeyboardElement& Keyboard::getKeyChangedStateThisFrame() const
{
    return m_changedThisFrame;
}

const KeyboardElement& Keyboard::getKeyDownThisFrame() const
{
    return m_keyDownThisFrame;
}

const KeyboardElement& Keyboard::getKeyUpThisFrame() const
{
    return m_keyUpThisFrame;
}