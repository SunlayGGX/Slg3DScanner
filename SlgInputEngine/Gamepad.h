#pragma once


namespace Slg3DScanner
{
    class Gamepad
    {
    public:
        using Buttons = decltype(XINPUT_GAMEPAD::wButtons);


    public:
        XINPUT_STATE m_currentState;

        /*******************************/
        //on pressed and on released
        /*******************************/
        Buttons m_buttonsChangedThisFrame;

        //On pressed keys
        Buttons m_buttonsDownThisFrame;

        //On released keys
        Buttons m_buttonsUpThisFrame;


    public:
        Gamepad();
        ~Gamepad();

        //update method. Return true if it has succeeded. False otherwise meaning the device was disconnected.
        bool update(Buttons& previousGamepadState);


        FORCEINLINE bool buttonIsPressed(Buttons button)        const { return (m_currentState.Gamepad.wButtons    & button) != 0; }
        FORCEINLINE bool button_OnPressed(Buttons button)       const { return (m_buttonsDownThisFrame             & button) != 0; }
        FORCEINLINE bool button_OnRelease(Buttons button)       const { return (m_buttonsUpThisFrame               & button) != 0; }
        FORCEINLINE bool button_OnStateChanged(Buttons button)  const { return (m_buttonsChangedThisFrame          & button) != 0; }


        template<Buttons buttonNum>
        constexpr bool isPressed() const { return (m_currentState.Gamepad.wButtons & this->makeButtonBitMask<buttonNum>()) != 0; }

        template<Buttons buttonNum>
        constexpr bool stateHasChangedThisFrame() const { return (m_buttonsChangedThisFrame & this->makeButtonBitMask<buttonNum>()) != 0; }

        template<Buttons buttonNum>
        constexpr bool wasPressedThisFrame() const { return (m_buttonsDownThisFrame & this->makeButtonBitMask<buttonNum>()) != 0; }

        template<Buttons buttonNum>
        constexpr bool wasReleasedThisFrame() const { return (m_buttonsUpThisFrame & this->makeButtonBitMask<buttonNum>()) != 0; }


        FORCEINLINE bool DPadUpIsPressed()              const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_DPAD_UP) != 0; }
        FORCEINLINE bool DPadUp_OnPressed()             const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_DPAD_UP) != 0; }
        FORCEINLINE bool DPadUp_OnReleased()            const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_DPAD_UP) != 0; }
        FORCEINLINE bool DPadUp_OnStateChanged()        const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_DPAD_UP) != 0; }

        FORCEINLINE bool DPadDownIsPressed()            const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_DPAD_DOWN) != 0; }
        FORCEINLINE bool DPadDown_OnPressed()           const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_DPAD_DOWN) != 0; }
        FORCEINLINE bool DPadDown_OnReleased()          const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_DPAD_DOWN) != 0; }
        FORCEINLINE bool DPadDown_OnStateChanged()      const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_DPAD_DOWN) != 0; }

        FORCEINLINE bool DPadLeftIsPressed()            const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_DPAD_LEFT) != 0; }
        FORCEINLINE bool DPadLeft_OnPressed()           const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_DPAD_LEFT) != 0; }
        FORCEINLINE bool DPadLeft_OnReleased()          const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_DPAD_LEFT) != 0; }
        FORCEINLINE bool DPadLeft_OnStateChanged()      const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_DPAD_LEFT) != 0; }

        FORCEINLINE bool DPadRightIsPressed()           const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_DPAD_RIGHT) != 0; }
        FORCEINLINE bool DPadRight_OnPressed()          const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_DPAD_RIGHT) != 0; }
        FORCEINLINE bool DPadRight_OnReleased()         const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_DPAD_RIGHT) != 0; }
        FORCEINLINE bool DPadRight_OnStateChanged()     const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_DPAD_RIGHT) != 0; }

        FORCEINLINE bool StartIsPressed()               const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_START) != 0; }
        FORCEINLINE bool Start_OnPressed()              const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_START) != 0; }
        FORCEINLINE bool Start_OnReleased()             const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_START) != 0; }
        FORCEINLINE bool Start_OnStateChanged()         const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_START) != 0; }

        FORCEINLINE bool BackIsPressed()                const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_BACK) != 0; }
        FORCEINLINE bool Back_OnPressed()               const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_BACK) != 0; }
        FORCEINLINE bool Back_OnReleased()              const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_BACK) != 0; }
        FORCEINLINE bool Back_OnStateChanged()          const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_BACK) != 0; }

        FORCEINLINE bool LeftThumbIsPressed()           const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_LEFT_THUMB) != 0; }
        FORCEINLINE bool LeftThumb_OnPressed()          const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_LEFT_THUMB) != 0; }
        FORCEINLINE bool LeftThumb_OnReleased()         const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_LEFT_THUMB) != 0; }
        FORCEINLINE bool LeftThumb_OnStateChanged()     const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_LEFT_THUMB) != 0; }

        FORCEINLINE bool RightThumbIsPressed()          const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_RIGHT_THUMB) != 0; }
        FORCEINLINE bool RightThumb_OnPressed()         const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_RIGHT_THUMB) != 0; }
        FORCEINLINE bool RightThumb_OnReleased()        const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_RIGHT_THUMB) != 0; }
        FORCEINLINE bool RightThumb_OnStateChanged()    const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_RIGHT_THUMB) != 0; }

        FORCEINLINE bool LeftShoulderIsPressed()        const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0; }
        FORCEINLINE bool LeftShoulder_OnPressed()       const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0; }
        FORCEINLINE bool LeftShoulder_OnReleased()      const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0; }
        FORCEINLINE bool LeftShoulder_OnStateChanged()  const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0; }

        FORCEINLINE bool RightShoulderIsPressed()       const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0; }
        FORCEINLINE bool RightShoulder_OnPressed()      const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0; }
        FORCEINLINE bool RightShoulder_OnReleased()     const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0; }
        FORCEINLINE bool RightShoulder_OnStateChanged() const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0; }

        FORCEINLINE bool AButtonIsPressed()             const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_A) != 0; }
        FORCEINLINE bool AButton_OnPressed()            const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_A) != 0; }
        FORCEINLINE bool AButton_OnReleased()           const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_A) != 0; }
        FORCEINLINE bool AButton_OnStateChanged()       const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_A) != 0; }

        FORCEINLINE bool BButtonIsPressed()             const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_B) != 0; }
        FORCEINLINE bool BButton_OnPressed()            const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_B) != 0; }
        FORCEINLINE bool BButton_OnReleased()           const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_B) != 0; }
        FORCEINLINE bool BButton_OnStateChanged()       const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_B) != 0; }

        FORCEINLINE bool XButtonIsPressed()             const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_X) != 0; }
        FORCEINLINE bool XButton_OnPressed()            const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_X) != 0; }
        FORCEINLINE bool XButton_OnReleased()           const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_X) != 0; }
        FORCEINLINE bool XButton_OnStateChanged()       const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_X) != 0; }

        FORCEINLINE bool YButtonIsPressed()             const { return (m_currentState.Gamepad.wButtons   & XINPUT_GAMEPAD_Y) != 0; }
        FORCEINLINE bool YButton_OnPressed()            const { return (m_buttonsDownThisFrame            & XINPUT_GAMEPAD_Y) != 0; }
        FORCEINLINE bool YButton_OnReleased()           const { return (m_buttonsUpThisFrame              & XINPUT_GAMEPAD_Y) != 0; }
        FORCEINLINE bool YButton_OnStateChanged()       const { return (m_buttonsChangedThisFrame         & XINPUT_GAMEPAD_Y) != 0; }


    private:
        void internalUpdateKeys(const Buttons& previousGamepadState);

        template<Buttons buttonNum>
        constexpr const Buttons makeButtonBitMask() const { return static_cast<Buttons>(~(0x1 << buttonNum)); }
    };
}
