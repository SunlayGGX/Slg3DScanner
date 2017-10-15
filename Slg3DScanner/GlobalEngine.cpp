#include "GlobalEngine.h"

#include "InputEngineMediator.h"
#include "SlgWindowMediator.h"
#include "LoggerEngine.h"

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
}

void GlobalEngine::destroy()
{
    this->quit();

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
            std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 / static_cast<int>(Slg3DScanner::Config::APPLICATION_FPS)/*MoonRPG::WINDOWS_THREAD_UPDATE_RATE_IN_MILLISECONDS*/ });
            inputMgrMediator.update();
            windowMgrMediator.update();
        }
    }}.detach();
}

bool GlobalEngine::isFullyInitialized() const
{
    return true;
        //RenderEngine::instance().isInitialized();
}