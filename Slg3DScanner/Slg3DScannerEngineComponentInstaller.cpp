#include "SlgSingletonGlobalAllocator.h"

#include "LoggerEngine.h"
#include "ArgumentManager.h"
#include "NameManager.h"
#include "DllManager.h"
#include "TimeManager.h"
#include "InputEngine.h"
#include "WindowManager.h"
#include "GlobalEngine.h"
#include "SlgRessourceManager.h"
#include "RenderEngineManager.h"
#include "RenderSceneManager.h"
#include "ShaderManager.h"
#include "SlgCoreEngine.h"
#include "TesterManager.h"

#include "SlgEngineCoreBinder.h"
#include "ThreadManager.h"

using namespace Slg3DScanner;

namespace Slg3DScanner
{
    using SlgSingletonContainerAllocatorAlias = SlgSingletonGlobalAllocator<
        LoggerEngine,
        ArgumentManager,
        NameManager,
        ThreadManager,
        TimeManager,
        DllManager,
        SlgCoreEngine,
        RessourceManager,
        InputEngine,
        WindowManager,
        RenderEngineManager,
        RenderSceneManager,
        ShaderManager,
        TesterManager,

        GlobalEngine,
        SlgEngineCoreBinder //must remain the last. It's role is to bind everyone together.
    >;

    void autoAllocateAndInstallSlgSingleton(int argc, char** argv)
    {
        ArgumentManager::registerArguments(argc, argv);

        static SlgSingletonContainerAllocatorAlias containerSlgSingletonAllocator;
    }
}
