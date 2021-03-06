#pragma once

#include "NonInstanciable.h"

#include "GlobalDef.h"

namespace Slg3DScanner
{
    class DefaultObjectCreator : private NonInstanciable
    {
    public:
        static IMeshRef createDefaultCubeMesh();
        static IMeshRef createDefaultPointCloud(const std::string &pathFileName);
    };
}
