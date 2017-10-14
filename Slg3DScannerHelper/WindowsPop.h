#pragma once

#include "NonInstanciable.h"

namespace Slg3DScanner
{
    class WindowsPop : private Slg3DScanner::NonInstanciable
    {
    public:
        static void popErrorWindow(const char* errorMessage);
    };
}
