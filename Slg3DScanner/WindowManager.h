#pragma once

#include "SlgSingleton.h"


namespace Slg3DScanner
{
    class WindowManager : private Slg3DScanner::SlgSingleton<WindowManager>
    {
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


    public:
        virtual void initialize() override;
        virtual void destroy() override;

        void update();

        HWND getWindowHandle() const;
        const TCHAR* getWindowTitleName() const;
        HACCEL getWindowAccelerationTable() const;
    };
}

