#pragma once

#include "SlgSingleton.h"


namespace Slg3DScanner
{
    class WindowManager : private Slg3DScanner::SlgSingleton<WindowManager>
    {
    public:
        using QuitDelegate = void(*)();
        using FinishInitializeDelegate = void(*)(HWND);


    private:
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(WindowManager);


    public:
        enum
        {
            MAX_TITLE_COUNT = 100,
        };


    private:
        HWND m_windowVisuHandle;
        TCHAR m_windowClass[Slg3DScanner::WindowManager::MAX_TITLE_COUNT];
        HACCEL m_accelerationTable;

        mutable std::mutex m_callbackMutex;
        QuitDelegate m_quitCallback;
        FinishInitializeDelegate m_finishInitializeCallback;


    public:
        void update();

        HWND getWindowHandle() const;
        const TCHAR* getWindowTitleName() const;
        HACCEL getWindowAccelerationTable() const;


    public:
        void bindQuitCallback(QuitDelegate callback);
        void bindFinishInitializeCallback(FinishInitializeDelegate callback, bool callNow);

        void callQuitCallback();
        void callFinishInitializeCallback();
        void unbindCallback();
    };
}

