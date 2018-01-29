#include "ThreadManager.h"


using namespace Slg3DScanner;

ThreadManager::ThreadManager()
{
    m_threadPool.reserve(16);
}

ThreadManager::~ThreadManager()
{
    this->destroy();
}

void ThreadManager::initialize()
{
    m_alive = true;
}

void ThreadManager::internalCleanThreadPool()
{
    const auto endIter = m_threadPool.end();
    for(auto iter = m_threadPool.begin(); iter != endIter; ++iter)
    {
        if(iter->joinable())
        {
            iter->join();
        }
    }
    m_threadPool.clear();
}

void ThreadManager::destroy()
{
    m_alive = false;

    std::lock_guard<std::mutex> autoLock{ m_mutex };
    this->internalCleanThreadPool();
}
