#include "SlgSurfaceComputationPCH.h"
#include "ComputationCloudStructureSimple.h"

#include "CloudVertex.h"
#include "InputCloudVertex.h"

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
    m_projPlaneNormal = { -scannerDir.x, -scannerDir.y, -scannerDir.z }; //normal face the camera (scanner)

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

void ComputationCloudStructureSimple::transferToFinal(std::vector<CloudVertex>& outFinalPointCloudArray) const
{
    //clean the vector before use.
    outFinalPointCloudArray.clear();
    outFinalPointCloudArray.reserve(m_cloudArray.size());

    //Transfer computationStructure point to final point
    const auto endArrayIter = m_cloudArray.end();
    for(auto arrayIter = m_cloudArray.begin(); arrayIter != endArrayIter; ++arrayIter)
    {
        outFinalPointCloudArray.emplace_back(arrayIter->m_position);
    }
}

void ComputationCloudStructureSimple::compute()
{
    if(m_cloudArray.size() < 3)
    {
        return;
    }

    this->computeProjectionOnPlane();
    this->computeDelaunayIndexes();
    this->mergeDelaunay();
}

void ComputationCloudStructureSimple::computeProjectionOnPlane()
{
    const auto endArrayIter = m_cloudArray.end();
    for(auto iter = m_cloudArray.begin(); iter != endArrayIter; ++iter)
    {
        iter->m_projectedPosition = { Slg3DScanner::scalar(iter->m_position, m_projPlaneRight), Slg3DScanner::scalar(iter->m_position, m_projPlaneUp) };
    }
}

void ComputationCloudStructureSimple::computeDelaunayIndexes()
{
    using CloudVertexType = decltype(m_cloudArray)::value_type;
    std::sort(m_cloudArray.begin(), m_cloudArray.end(), [](const CloudVertexType& cloudVertex1, const CloudVertexType& cloudVertex2)
    {
        if(cloudVertex1.m_projectedPosition.x < cloudVertex2.m_projectedPosition.x)
        {
            return true;
        }
        else if(cloudVertex1.m_projectedPosition.x == cloudVertex2.m_projectedPosition.x)
        {
            return cloudVertex1.m_projectedPosition.y < cloudVertex2.m_projectedPosition.y;
        }

        return false;
    });
}

void ComputationCloudStructureSimple::mergeDelaunay()
{
    std::list<DelaunayTriangle> outMergeContainer;
    this->mergeDelaunayRecursive(0, m_cloudArray.size() - 1, outMergeContainer);

    m_indexTriangle.reserve(outMergeContainer.size());
    auto endIter = outMergeContainer.end();
    for(auto iter = outMergeContainer.begin(); iter != endIter; ++iter)
    {
        m_indexTriangle.emplace_back(*iter);
    }
}

void ComputationCloudStructureSimple::mergeDelaunayRecursive(std::size_t beginIndex, std::size_t endIndex, std::list<DelaunayTriangle>& outMergeContainer)
{
    std::size_t diff = endIndex - beginIndex + 1;
    if(diff > 3)
    {
        std::list<DelaunayTriangle> mergeContainer1;
        std::list<DelaunayTriangle> mergeContainer2;

        std::size_t half = beginIndex + diff / 2;

        this->mergeDelaunayRecursive(beginIndex, half, mergeContainer1);
        this->mergeDelaunayRecursive(half + 1, endIndex, mergeContainer2);

        this->mergeSubsetsPointsIntoOne(mergeContainer1, mergeContainer2, outMergeContainer);
    }
    else
    {
        if(diff == 1)
        {
            outMergeContainer.emplace_back(beginIndex);
        }
        else if(diff == 2)
        {
            outMergeContainer.emplace_back(beginIndex, endIndex);
        }
        else
        {
            outMergeContainer.emplace_back(beginIndex, beginIndex + 1, endIndex);
        }
    }
}

void ComputationCloudStructureSimple::mergeSubsetsPointsIntoOne(std::list<DelaunayTriangle>& inContainer1, std::list<DelaunayTriangle>& inContainer2, std::list<DelaunayTriangle>& outMergeContainer)
{
    //TODO
    outMergeContainer.splice(outMergeContainer.end(), inContainer1);
    outMergeContainer.splice(outMergeContainer.end(), inContainer2);
}
