#include "GlobalEngine.h"

#include "InputEngineMediator.h"
#include "SlgWindowMediator.h"
#include "LoggerEngine.h"
#include "RenderEngineManager.h"
#include "TimeManager.h"

#include "Slg3DScannerConfig.h"

using namespace Slg3DScanner;


GlobalEngine::GlobalEngine() :
    m_run{ false }
{

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
}

void GlobalEngine::destroy()
{
    this->quit();

    TimeManager::instance().destroy();
    //RenderEngine::instance().destroy();
}

void GlobalEngine::run()
{
    constexpr const std::chrono::milliseconds updateTime{ 1000 / static_cast<int>(Slg3DScanner::Config::APPLICATION_FPS) };

    while(m_run && !this->isFullyInitialized())
    {
        std::this_thread::sleep_for(updateTime);
    }

    while(m_run)
    {
        /*TODO : Update the Game Loop*/

        //RenderEngine::instance().update();

        //Bad, but until we have a proper TimeManager, this will do the job...
        //TODO : Call a proper method from a Time Manager or Synchronize with VSync from RenderEngine swap chain present (present1 method) or so
        std::this_thread::sleep_for(updateTime);
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
    RenderEngineManager::instance().initializeDevice(windowVisuHandle);
    RenderEngineManager::instance().initialize();

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
    return true;
        //RenderEngine::instance().isInitialized();
}