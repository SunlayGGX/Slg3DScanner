#pragma once

#include "SlgSingleton.h"

namespace Slg3DScanner
{
    class SlgCoreEngine : private Slg3DScanner::SlgSingleton<SlgCoreEngine>
    {
    private:
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(SlgCoreEngine);


    public:
        void runTest();
    };
}
