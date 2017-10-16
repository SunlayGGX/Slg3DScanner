#pragma once

#include "SlgSingleton.h"


namespace Slg3DScanner
{
    class GlobalEngine : private Slg3DScanner::SlgSingleton<GlobalEngine>
    {
    private:
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(GlobalEngine);


    public:
        std::atomic<bool> m_run;


    public:
        virtual void initialize() override;
        virtual void destroy() override;

        void run();

        void quit();

        bool isFullyInitialized() const;

        void startRendering(HWND windowVisuHandle);


    public:
        void arrangeObjectInSceneWorld();


    private:
        void startInputAndWindowsThread() const;
        void internalStartRenderThread() const;
    };
}

