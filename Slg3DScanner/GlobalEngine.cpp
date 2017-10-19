#include "GlobalEngine.h"

#include "BitHelper.h"

#include "InputEngineMediator.h"
#include "SlgWindowMediator.h"
#include "LoggerEngine.h"
#include "RenderEngineManager.h"
#include "TimeManager.h"
#include "SlgCoreEngine.h"

#include "Slg3DScannerConfig.h"

#include "ITask.h"
#include "InputTask.h"

using namespace Slg3DScanner;


GlobalEngine::GlobalEngine() :
    m_run{ false }
{
    m_taskMap[TaskOrder::INPUT] = std::make_unique<InputTask>();
}

GlobalEngine::~GlobalEngine()
{

}

void GlobalEngine::initialize()
{
    //RenderEngine::instance().initialize();
    if(m_run)
    {
        return;
    }

    m_run = true;

    this->startInputAndWindowsThread();

    TimeManager::instance().initialize();
    SlgCoreEngine::instance().initialize();

    this->internalInitializeAllTasks();
}

void GlobalEngine::destroy()
{
    this->quit();

    this->internalDestroyAllTasks();

    TimeManager::instance().destroy();
    SlgCoreEngine::instance().destroy();
}

void GlobalEngine::run()
{
    constexpr const std::chrono::milliseconds updateTime{ 1000 / static_cast<int>(Slg3DScanner::Config::APPLICATION_FPS) };

    while(m_run && !this->isFullyInitialized())
    {
        std::this_thread::sleep_for(updateTime);
    }

    SlgCoreEngine::instance().runTest();

    auto endTaskIter = m_taskMap.end();
    while(m_run)
    {
        for(auto iter = m_taskMap.begin(); iter != endTaskIter; ++iter)
        {
            iter->second->update();
        }

        TimeManager::instance().waitEndOfFrame();
    }
}

void GlobalEngine::quit()
{
    m_run = false;
}

void GlobalEngine::startInputAndWindowsThread() const
{
    std::thread
    {
        [&run = this->m_run]() 
    {
        SlgWindowMediator windowMgrMediator(&LoggerEngine::instance());
        InputEngineMediator inputMgrMediator(&LoggerEngine::instance(), windowMgrMediator.getWindowHandle());

        while(run)
        {
            inputMgrMediator.update();
            windowMgrMediator.update();
            TimeManager::instance().waitEndOfFrame();
        }
    }}.detach();
}

void GlobalEngine::startRendering(HWND windowVisuHandle)
{
    RenderEngineManager& renderMgr = RenderEngineManager::instance();

    renderMgr.initializeDevice(windowVisuHandle);
    renderMgr.initialize();

    m_allInitialized |= BitMask<GlobalEngine::RENDER_INITIALIZED>::value;

    this->internalStartRenderThread();
}

void GlobalEngine::internalStartRenderThread() const
{
    std::thread
    {
        [&run = this->m_run]()
    {
        RenderEngineManager& renderMgr = RenderEngineManager::instance();
        while(run)
        {
            renderMgr.update();
            TimeManager::instance().waitEndOfFrame();
        }

        renderMgr.destroy();
    }}.detach();
}

bool GlobalEngine::isFullyInitialized() const
{
    return m_allInitialized == BitFill<GlobalEngine::FULL_INITIALIZED>::value;
}

void GlobalEngine::internalInitializeAllTasks()
{
    auto endTaskIter = m_taskMap.end();
    for(auto iter = m_taskMap.begin(); iter != endTaskIter; ++iter)
    {
        iter->second->initialize();
    }
}

void GlobalEngine::internalDestroyAllTasks()
{
    auto endTaskIter = m_taskMap.end();
    for(auto iter = m_taskMap.begin(); iter != endTaskIter; ++iter)
    {
        iter->second->destroy();
    }
}
