#include "SlgCoreEnginePCH.h"
#include "TesterManager.h"

using namespace Slg3DScanner;

TesterManager::TesterManager()
{
}

TesterManager::~TesterManager()
{
}

void TesterManager::initialize()
{

}

void TesterManager::destroy()
{

}

std::chrono::microseconds TesterManager::getWaitingValueBetweenTestQueue() const
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    return m_waitValueBetweenTests;
}

void TesterManager::setWaitingValueBetweenTestQueue(const std::chrono::microseconds newValue)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    m_waitValueBetweenTests = newValue;
}

void TesterManager::disableWaitingValueBetweenTestQueue()
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    m_waitValueBetweenTests = std::chrono::microseconds{};
}

void TesterManager::executeWaitBetweenTest()
{
    std::chrono::microseconds toWait;
    {
        std::lock_guard<std::mutex> autoLocker{ m_mutex };
        if(m_waitValueBetweenTests == std::chrono::microseconds{})
        {
            return;
        }

        toWait = m_waitValueBetweenTests;
    }

    std::this_thread::sleep_for(toWait);
}
