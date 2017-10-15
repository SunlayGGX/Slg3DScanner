#include "SlgInputEnginePCH.h"

#include "InputEngineMediator.h"

#include "InputEngine.h"
#include "LoggerBind.h"

using namespace Slg3DScanner;

InputEngineMediator::InputEngineMediator(LoggerEngine* instance, HWND windowHandle)
{
    LoggerBind::bindToExistant(instance);

    InputEngine& inputMgr = InputEngine::instance();
    inputMgr.initialize();
    inputMgr.createKeyboard(windowHandle);
}

InputEngineMediator::~InputEngineMediator()
{
    InputEngine::instance().destroy();
}

void InputEngineMediator::update()
{
    InputEngine::instance().update();
}
