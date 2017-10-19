#pragma once

#include "SlgSingleton.h"


namespace Slg3DScanner
{
    class GlobalEngine : private Slg3DScanner::SlgSingleton<GlobalEngine>
    {
    private:
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(GlobalEngine);


    private:
        enum
        {
            RENDER_INITIALIZED,

            FULL_INITIALIZED
        };


    public:
        std::atomic<bool> m_run;

        std::atomic<uint8_t> m_allInitialized;


    public:
        void run();

        void quit();

        bool isFullyInitialized() const;

        void startRendering(HWND windowVisuHandle);


    private:
        void startInputAndWindowsThread() const;
        void internalStartRenderThread() const;
    };
}

