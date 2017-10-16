#include "SlgWindowPCH.h"
#include "SlgWindowMediator.h"

#include "LoggerBind.h"
#include "WindowManager.h"

using namespace Slg3DScanner;

SlgWindowMediator::SlgWindowMediator(LoggerEngine* instance)
{
    LoggerBind::bindToExistant(instance);
    WindowManager::instance().initialize();
}

SlgWindowMediator::~SlgWindowMediator()
{
    WindowManager::instance().destroy();
}

void SlgWindowMediator::update()
{
    WindowManager::instance().update();
}

HWND SlgWindowMediator::getWindowHandle() const
{
    return WindowManager::instance().getWindowHandle();
}
