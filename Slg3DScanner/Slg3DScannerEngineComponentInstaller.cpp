#include "SlgSingletonGlobalAllocator.h"

#include "LoggerEngine.h"
#include "NameManager.h"
#include "DllManager.h"
#include "TimeManager.h"
#include "InputEngine.h"
#include "WindowManager.h"
#include "GlobalEngine.h"
#include "RenderEngineManager.h"
#include "RenderSceneManager.h"
#include "ShaderManager.h"

#include "SlgEngineCoreBinder.h"

using namespace Slg3DScanner;

namespace Slg3DScanner
{
    using SlgSingletonContainerAllocatorAlias = SlgSingletonGlobalAllocator<
        LoggerEngine,
        NameManager,
        TimeManager,
        DllManager,
        InputEngine,
        WindowManager,
        RenderEngineManager,
        RenderSceneManager,
        ShaderManager,

        GlobalEngine,
        SlgEngineCoreBinder //must remain the last. It's role is to bind everyone together.
    >;

    void autoAllocateAndInstallSlgSingleton()
    {
        static SlgSingletonContainerAllocatorAlias containerSlgSingletonAllocator;
    }
}
