#pragma once


#include "Uncopyable.h"

namespace Slg3DScanner
{
    class Scheduler : private Uncopyable{};

    class DelayScheduler : public Scheduler
    {
    public:
        template<class TimeDelay, class Algorithm, class ... Args>
        DelayScheduler(TimeDelay delay, Algorithm&& func, Args&& ... args)
        {
            std::thread(
                [delay, func = std::forward<Algorithm>(func), args...]()
            {
                std::this_thread::sleep_for(delay);

                func(std::forward<Args>(args)...);
            }).detach();
        }
    };

    class LoopScheduler : public Scheduler
    {
    private:
        std::shared_ptr<std::atomic<bool>> m_run;


    public:
        template<class TimeLoop, class Algorithm, class ... Args>
        LoopScheduler(const bool tightLoop, TimeLoop time, Algorithm&& func, Args&& ... args) :
            m_run{ std::make_shared<std::atomic<bool>>(true) }
        {
            if(tightLoop)
            {
                std::thread(
                    [time, run = this->m_run, func = std::forward<Algorithm>(func), args...]()
                {
                    constexpr const std::chrono::microseconds deltaMin{ 1 };

                    while(*run)
                    {
                        auto begin = std::chrono::high_resolution_clock::now();

                        func(std::forward<Args>(args)...);

                        auto executionDelta = time - (std::chrono::high_resolution_clock::now() - begin);

                        if(executionDelta > deltaMin)
                        {
                            std::this_thread::sleep_for(executionDelta);
                        }
                    }
                }).detach();
            }
            else
            {
                std::thread(
                    [time, run = this->m_run, func = std::forward<Algorithm>(func), args...]()
                {
                    while(*run)
                    {
                        func(std::forward<Args>(args)...);

                        std::this_thread::sleep_for(time);
                    }
                }).detach();
            }
        }

        LoopScheduler(LoopScheduler&& scheduler);

        ~LoopScheduler();

        void kill();
        bool isKilled() const;
        bool isValid() const;
    };
}