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

    //Transfer input point to final point
    for(std::size_t iter = 0; iter != inputedVertexCount; ++iter)
    {
        outFinalCloudVertexVector.emplace_back(inInputedVertexes[iter].m_vertex);
    }

    //Compute normals
}