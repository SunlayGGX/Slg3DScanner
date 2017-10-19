#pragma once

#include "SlgSingleton.h"

namespace Slg3DScanner
{
    class RessourceManager : private Slg3DScanner::SlgSingleton<RessourceManager>
    {
    private:
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(RessourceManager);


    private:


    };
}
