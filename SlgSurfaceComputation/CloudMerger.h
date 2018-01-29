#pragma once

#include "NonInstanciable.h"

namespace Slg3DScanner
{
    struct InternalCloudMesh;

    class CloudMerger : private Slg3DScanner::NonInstanciable
    {
    public:
        static void merge(InternalCloudMesh& to, InternalCloudMesh&& from);
    };
}

