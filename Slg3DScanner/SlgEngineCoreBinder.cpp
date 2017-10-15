#include "Slg3DScannerPCH.h"
#include "SlgEngineCoreBinder.h"

#include "GlobalEngine.h"
#include "WindowManager.h"

#include "LoggerCommand.h"

using namespace Slg3DScanner;

    
SlgEngineCoreBinder::SlgEngineCoreBinder()
{
    WindowManager::instance().bindQuitCallback(
        []() 
    {
        GlobalEngine::instance().quit();
    });

    LoggerCommand::initialize();

    GlobalEngine::instance().initialize();
}

SlgEngineCoreBinder::~SlgEngineCoreBinder()
{
    GlobalEngine::instance().destroy();
}

void SlgEngineCoreBinder::initialize()
{

}

void SlgEngineCoreBinder::destroy()
{

}
