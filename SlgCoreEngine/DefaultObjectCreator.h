#pragma once

#include "NonInstanciable.h"

#include "GlobalDef.h"

namespace Slg3DScanner
{
    class DefaultObjectCreator : private NonInstanciable
    {
    public:
        static void createDefaultCamera(const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& direction, const DirectX::XMVECTOR& up);

        static IMeshRef createDefaultCubeMesh();
        static IMeshRef createDefaultPointCloud(const std::string &pathFileName, int cloudVersion);
    };
}
