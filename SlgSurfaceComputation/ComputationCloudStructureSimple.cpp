#include "SlgSurfaceComputationPCH.h"
#include "ComputationCloudStructureSimple.h"

#include "CloudVertex.h"
#include "InputCloudVertex.h"

#include "DelaunayStructure.h"
#include "DelaunayComputator.h"

#include "SlgMath.h"

#include <algorithm>

using namespace Slg3DScanner;

ComputationCloudStructureSimple::ComputationCloudStructureSimple(const DirectX::XMFLOAT3& scannerDir, const InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount)
{
    this->initializeProjectivePlane(scannerDir);
    this->fillArray(inInputedVertexes, inputedVertexCount);
}

ComputationCloudStructureSimple::~ComputationCloudStructureSimple()
{
}

void ComputationCloudStructureSimple::initializeProjectivePlane(const DirectX::XMFLOAT3& scannerDir)
{
    m_projPlaneOrigin = { 0.f, 0.f, 0.f };
    m_projPlaneNormal = { scannerDir.x, scannerDir.y, scannerDir.z }; //normal face the camera (scanner)

    float norm = std::sqrtf(Slg3DScanner::scalar(m_projPlaneNormal, m_projPlaneNormal));

    if(norm > 0.0000001f)
    {
        m_projPlaneNormal.x /= norm;
        m_projPlaneNormal.y /= norm;
        m_projPlaneNormal.z /= norm;
    }
    else
    {
        m_projPlaneNormal = { 0.f, 0.f, 1.f };
    }

    DirectX::XMFLOAT3 firstVect = { 0.f, 1.f, 0.f };
    if(std::abs(Slg3DScanner::scalar(firstVect, m_projPlaneNormal) - 1.f) < 0.0000001f)
    {
        firstVect = { 0.f, 0.f, 1.f };
    }

    m_projPlaneRight = Slg3DScanner::getNormalized(Slg3DScanner::cross(m_projPlaneNormal, firstVect));
    m_projPlaneUp = Slg3DScanner::getNormalized(Slg3DScanner::cross(m_projPlaneRight, m_projPlaneNormal));
}

void ComputationCloudStructureSimple::fillArray(const InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount)
{
    m_cloudArray.clear();
    m_cloudArray.reserve(inputedVertexCount);

    for(std::size_t iter = 0; iter != inputedVertexCount; ++iter)
    {
        auto& inputVertex = inInputedVertexes[iter];

        m_cloudArray.emplace_back(inputVertex);
    }
}

void ComputationCloudStructureSimple::transferToFinal(InternalCloudMesh& outFinalPointCloudArray) const
{
    //clean the vertex vector before use.
    outFinalPointCloudArray.m_vertexes.clear();
    outFinalPointCloudArray.m_vertexes.reserve(m_cloudArray.size());

    //Transfer computationStructure point to final vertex
    const auto endArrayIter = m_cloudArray.end();
    for(auto vertexIter = m_cloudArray.begin(); vertexIter != endArrayIter; ++vertexIter)
    {
        outFinalPointCloudArray.m_vertexes.emplace_back(vertexIter->m_position);
    }

    //clean the vertex vector before use.
    outFinalPointCloudArray.m_indexes.clear();
    outFinalPointCloudArray.m_indexes.reserve(m_indexTriangle.size() * 3);

    //Transfer computationStructure point to final index
    const auto endIndexIter = m_indexTriangle.end();
    for(auto indexIter = m_indexTriangle.begin(); indexIter != endIndexIter; ++indexIter)
    {
        outFinalPointCloudArray.m_indexes.emplace_back(indexIter->index0);
        outFinalPointCloudArray.m_indexes.emplace_back(indexIter->index1);
        outFinalPointCloudArray.m_indexes.emplace_back(indexIter->index2);
    }
}

void ComputationCloudStructureSimple::compute()
{
    if(m_cloudArray.size() < 3)
    {
        return;
    }

    this->computeProjectionOnPlane();
    this->computeDelaunay();
}

void ComputationCloudStructureSimple::computeProjectionOnPlane()
{
    const auto endArrayIter = m_cloudArray.end();
    for(auto iter = m_cloudArray.begin(); iter != endArrayIter; ++iter)
    {
        iter->m_projectedPosition = { Slg3DScanner::scalar(iter->m_position, m_projPlaneRight), Slg3DScanner::scalar(iter->m_position, m_projPlaneUp) };
    }
}

void ComputationCloudStructureSimple::computeDelaunay()
{
    DelaunayComputator computator{ m_cloudArray };
    computator.compute<DelaunayComputator::DivideAndConquer>();

    const std::list<DelaunayTriangle>& triangleList = computator.getTriangleList();

    m_indexTriangle.clear();
    m_indexTriangle.reserve(triangleList.size());
    std::copy(triangleList.begin(), triangleList.end(), std::back_inserter(m_indexTriangle));
}
