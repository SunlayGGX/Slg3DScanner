#pragma once

#include "SlgSingleton.h"


namespace Slg3DScanner
{
    class ITask;

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

        enum TaskOrder
        {
            INPUT
        };


    public:
        std::atomic<bool> m_run;

        std::map<TaskOrder, std::unique_ptr<ITask>> m_taskMap;

        std::atomic<uint8_t> m_allInitialized;

        std::thread m_renderThread;
        std::thread m_inputThread;


    public:
        void run();

        void quit();

        bool isFullyInitialized() const;

        void startRendering(HWND windowVisuHandle);


    private:
        void startInputAndWindowsThread();
        void internalStartRenderThread();

        void internalInitializeAllTasks();
        void internalDestroyAllTasks();
    };
}

