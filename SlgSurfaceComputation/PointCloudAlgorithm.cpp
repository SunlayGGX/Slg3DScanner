#include "SlgSurfaceComputationPCH.h"
#include "PointCloudAlgorithm.h"

#include "CloudVertex.h"
#include "CloudVertexComputationStructure.h"
#include "CloudVertexComputationStructureSimple.h"
#include "InputCloudVertex.h"

#include "PointCloudComputator.h"



#include "ComputationCloudStructure.h"
#include "ComputationCloudStructureSimple.h"

using namespace Slg3DScanner;

void PointCloudAlgorithm::computeInputCloudVertexToFinalCloudVertex(std::vector<Slg3DScanner::CloudVertex>& outFinalCloudVertexVector, const Slg3DScanner::InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount)
{
    ComputationCloudStructure cloud{ inInputedVertexes, inputedVertexCount };
    
    cloud.compute();

    cloud.transferToFinal(outFinalCloudVertexVector);

    //std::vector<CloudVertexComputationStructure> computationMeshCloud;
    //computationMeshCloud.reserve(inputedVertexCount);

    ////Transfer input point to computationStructure
    //for(std::size_t iter = 0; iter != inputedVertexCount; ++iter)
    //{
    //    computationMeshCloud.emplace_back(inInputedVertexes[iter]);
    //}

    ////create computator for mesh cloud and computeMeshCloud
    //PointCloudComputator{ computationMeshCloud }();

    ////clean the vector before use.
    //outFinalCloudVertexVector.clear();
    //outFinalCloudVertexVector.reserve(inputedVertexCount);

    ////Transfer computationStructure point to final point
    //auto endIter = computationMeshCloud.end();
    //for(auto iter = computationMeshCloud.begin(); iter != endIter; ++iter)
    //{
    //    outFinalCloudVertexVector.emplace_back(iter->m_position);
    //}
}

void PointCloudAlgorithm::computeInputCloudVertexToFinalCloudVertexSimple(std::vector<Slg3DScanner::CloudVertex>& outFinalCloudVertexVector, const Slg3DScanner::InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount, DirectX::XMFLOAT3& scannerDir)
{
    ComputationCloudStructureSimple cloud{ scannerDir, inInputedVertexes, inputedVertexCount };

    cloud.compute();

    cloud.transferToFinal(outFinalCloudVertexVector);
}
