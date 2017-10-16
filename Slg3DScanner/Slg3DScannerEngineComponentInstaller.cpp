#include "SlgSingletonGlobalAllocator.h"

#include "LoggerEngine.h"
#include "DllManager.h"
#include "TimeManager.h"
#include "InputEngine.h"
#include "WindowManager.h"
#include "GlobalEngine.h"

#include "SlgEngineCoreBinder.h"

using namespace Slg3DScanner;

namespace Slg3DScanner
{
    using SlgSingletonContainerAllocatorAlias = SlgSingletonGlobalAllocator<
        LoggerEngine,
        TimeManager,
        DllManager,
        InputEngine,
        WindowManager,

        GlobalEngine,
        SlgEngineCoreBinder //must remain the last. It's role is to bind everyone together.
    >;

    void autoAllocateAndInstallSlgSingleton()
    {
        static SlgSingletonContainerAllocatorAlias containerSlgSingletonAllocator;
    }
}
