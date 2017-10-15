#pragma once

#include "SlgSingleton.h"

namespace Slg3DScanner
{
    /*
    It's role is to bind all application engine component pieces together
    */
    class SlgEngineCoreBinder : private Slg3DScanner::SlgSingleton<SlgEngineCoreBinder>
    {
    private:
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(SlgEngineCoreBinder);


    public:
        virtual void initialize() override;
        virtual void destroy() override;
    };
}
