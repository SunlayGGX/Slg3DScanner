#pragma once

#include "SlgSingleton.h"


namespace Slg3DScanner
{
    class ThreadManager : private Slg3DScanner::SlgSingleton<ThreadManager>
    {
    private:
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(ThreadManager);

    private:
        mutable std::mutex m_mutex;

        std::vector<std::thread> m_threadPool;

        std::atomic<bool> m_alive;

    private:
        void internalCleanThreadPool();

    public:
        template<class Func, class ... Args>
        void addFunctionToRunAsynchronously(Func&& func, Args&& ... args)
        {
            if(m_alive)
            {
                std::lock_guard<std::mutex> autoLock{ m_mutex };
                m_threadPool.emplace_back(std::thread{ std::forward<Func>(func), std::forward<Args>(args)... });
            }
        }
    };
}
