#include "TimeManager.h"

#include "EngineConfig.h"

#include "AutoSchedulerResolutionModifier.h"


using namespace Slg3DScanner;


TimeManager::TimeManager() :
    m_engineStart{ std::chrono::high_resolution_clock::now() },
    m_frameDuration{ 1000000 / static_cast<int>(Slg3DScanner::Constants::FPS) },
    m_canHookOnTimeManager{ false },
    m_run{ false }
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::initialize()
{
    if(!m_run)
    {
        m_run = true;

        this->internalUpdateTimes();

        std::thread(
            [this]() {

            AutoSchedulerResolutionModifier<> autoResolutionModifier;

            while(m_run)
            {
                std::this_thread::sleep_for(this->getRemainingFrameTime());

                this->internalUpdateTimes();
                m_synchronizer.notify_all();
            }
        }).detach();

        m_canHookOnTimeManager = true;
    }
}

void TimeManager::destroy()
{
    m_run = false;
    m_canHookOnTimeManager = false;

    m_synchronizer.notify_all();

    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    m_loopSchedulerArray.clear();
}

void TimeManager::internalUpdateTimes()
{
    auto lastFrameStart = m_frameStart;

    m_frameStart = std::chrono::high_resolution_clock::now();
    m_approximativeFrameEnd = m_frameStart + m_frameDuration;

    m_lastFrameDurationInSeconds = std::chrono::duration_cast<std::chrono::duration<float, std::chrono::seconds::period>>(m_frameStart - lastFrameStart).count();
}

void TimeManager::startGame()
{
    m_gameStart = std::chrono::high_resolution_clock::now();
}

float TimeManager::getLastFrameDurationInSeconds() const
{
    return m_lastFrameDurationInSeconds;
}

std::chrono::nanoseconds TimeManager::getRemainingFrameTime() const
{
    constexpr const std::chrono::nanoseconds noTime = std::chrono::nanoseconds{ 0 };

    std::chrono::nanoseconds result = m_approximativeFrameEnd - std::chrono::high_resolution_clock::now();
    
    if(result < noTime)
    {
        return noTime;
    }

    return result;
}

std::chrono::nanoseconds TimeManager::getElapsedTimeSinceEngineStarting() const
{
    return std::chrono::high_resolution_clock::now() - m_engineStart;
}

std::chrono::nanoseconds TimeManager::getElapsedTimeSinceGameStarting() const
{
    return std::chrono::high_resolution_clock::now() - m_gameStart;
}

void TimeManager::waitEndOfFrame()
{
    if(m_canHookOnTimeManager)
    {
        m_synchronizer.wait(std::unique_lock<std::mutex>{ m_syncMutex });
    }
}
