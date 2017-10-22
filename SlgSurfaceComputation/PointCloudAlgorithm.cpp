#include "SlgSurfaceComputationPCH.h"
#include "PointCloudAlgorithm.h"

#include "CloudVertex.h"
#include "InputCloudVertex.h"

using namespace Slg3DScanner;

void PointCloudAlgorithm::computeInputCloudVertexToFinalCloudVertex(std::vector<Slg3DScanner::CloudVertex>& outFinalCloudVertexVector, const Slg3DScanner::InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount)
{
    //clean the vector before use.
    outFinalCloudVertexVector.clear();
    outFinalCloudVertexVector.reserve(inputedVertexCount);


    //for now (because the final algorithm is not implemented yet. To allow retrocompatibility with what we had before)
    for(std::size_t iter = 0; iter != inputedVertexCount; ++iter)
    {
        auto& inputVertex = inInputedVertexes[iter];
        outFinalCloudVertexVector.emplace_back(inputVertex.m_vertex.x, inputVertex.m_vertex.y, inputVertex.m_vertex.z);
    }
}