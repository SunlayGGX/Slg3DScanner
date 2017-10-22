#pragma once

#include "NonInstanciable.h"


namespace Slg3DScanner
{
    struct CloudVertex;
    struct InputCloudVertex;
}

namespace Slg3DScanner
{
    class PointCloudAlgorithm : private NonInstanciable
    {
    public:
        static void computeInputCloudVertexToFinalCloudVertex(std::vector<Slg3DScanner::CloudVertex>& outFinalCloudVertexVector, const Slg3DScanner::InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount);
    };
}
