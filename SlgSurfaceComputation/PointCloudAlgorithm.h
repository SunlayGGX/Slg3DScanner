#pragma once

#include "NonInstanciable.h"


namespace Slg3DScanner
{
    struct CloudVertex;
    struct InternalCloudMesh;
    struct InputCloudVertex;
}

namespace Slg3DScanner
{
    class PointCloudAlgorithm : private NonInstanciable
    {
    public:
        static void computeInputCloudVertexToFinalCloudVertex(std::vector<Slg3DScanner::CloudVertex>& outFinalCloudVertexVector, const Slg3DScanner::InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount);

        static void computeInputCloudVertexToFinalCloudVertexSimple(Slg3DScanner::InternalCloudMesh& outFinalCloudMesh, const Slg3DScanner::InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount, DirectX::XMFLOAT3& scannerDir);
    };
}
