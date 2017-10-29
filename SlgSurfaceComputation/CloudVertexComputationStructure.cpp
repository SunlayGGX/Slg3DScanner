#include "SlgSurfaceComputationPCH.h"
#include "CloudVertexComputationStructure.h"

#include "InputCloudVertex.h"

#include "ComputationCloudStructure.h"

#include "SlgMath.h"

#include <algorithm>

using namespace Slg3DScanner;


CloudVertexComputationStructure::CloudVertexComputationStructure(const InputCloudVertex& inputVertex, ComputationCloudBox& box) :
    m_position{ inputVertex.m_vertex },
    m_distanceKnnSquared{ CloudVertexComputationStructure::DEFAULT_KNN_DISTANCE_SQUARED },
    m_mutex{ std::make_shared<std::mutex>() },
    m_containingBox{ &box }
{
}

CloudVertexComputationStructure::CloudVertexComputationStructure(CloudVertexComputationStructure&) = default;
CloudVertexComputationStructure& CloudVertexComputationStructure::operator=(const CloudVertexComputationStructure&) = default;

CloudVertexComputationStructure& CloudVertexComputationStructure::operator=(CloudVertexComputationStructure&& other)
{
    this->swap(CloudVertexComputationStructure{ std::forward<CloudVertexComputationStructure>(other)});
    return *this;
}

CloudVertexComputationStructure::CloudVertexComputationStructure(CloudVertexComputationStructure&& other) :
    m_distanceKnnSquared{ other.m_distanceKnnSquared },
    m_tangentPlaneNormal{ other.m_tangentPlaneNormal },
    m_tangentPlaneOrigin{ other.m_tangentPlaneOrigin },
    m_position{ other.m_position },
    m_containingBox{ other.m_containingBox }
{
    std::swap(other.m_mutex, m_mutex);
    std::swap(other.m_neighbours, m_neighbours);
}

CloudVertexComputationStructure::~CloudVertexComputationStructure()
{
}

void CloudVertexComputationStructure::swap(CloudVertexComputationStructure& other)
{
    std::swap(other.m_containingBox, m_containingBox);
    std::swap(other.m_position, m_position);
    std::swap(m_distanceKnnSquared, other.m_distanceKnnSquared);
    std::swap(m_tangentPlaneNormal, other.m_tangentPlaneNormal);
    std::swap(m_tangentPlaneOrigin, other.m_tangentPlaneOrigin);
    std::swap(other.m_neighbours, m_neighbours);
    std::swap(other.m_mutex, m_mutex);
}

ComputationCloudBox* CloudVertexComputationStructure::getContainingBox() const
{
    return m_containingBox;
}

void CloudVertexComputationStructure::addNeighbour(CloudVertexComputationStructure* neighbour)
{
    std::lock_guard<std::mutex> autoLocker{ *m_mutex };
    m_neighbours.emplace_back(neighbour);
}

void CloudVertexComputationStructure::optimizeAllocSpeed(std::size_t newNeighbourCapacity)
{
    std::lock_guard<std::mutex> autoLocker{ *m_mutex };
    m_neighbours.reserve(m_neighbours.capacity() + newNeighbourCapacity);
}

void CloudVertexComputationStructure::optimizeAllocMemory()
{
    m_neighbours.shrink_to_fit();
}

void CloudVertexComputationStructure::eraseNeighbourNoDoublon()
{
    if(m_neighbours.size() < 2)
    {
        return;
    }

    std::size_t toErase = 0;

    auto endIter = m_neighbours.end() - 1;
    for(auto iter = m_neighbours.begin(); iter != endIter; ++iter)
    {
        //the algorithme was made for point inside a same box cannot be doublons. So shortcut...
        if(this->m_containingBox == (*iter)->m_containingBox)
        {
            continue;
        }

        auto endSearch = endIter + 1;
        auto found = std::find(iter + 1, endSearch, *iter);  ;
        while(found != endSearch)
        {
            ++toErase;
            std::swap(*found, *endIter);
            --endIter;
            endSearch = endIter + 1;
            found = std::find(found, endSearch, *iter);
        }
    };

    while(toErase != 0)
    {
        m_neighbours.pop_back();
        --toErase;
    }
}

void CloudVertexComputationStructure::evaluateNeighbourhood(CloudVertexComputationStructure& newNeighbour)
{
    DirectX::XMFLOAT3 m_neighbourVect = { newNeighbour.m_position.x - m_position.x, newNeighbour.m_position.y - m_position.y, newNeighbour.m_position.z - m_position.z };

    float distanceSquared = m_neighbourVect.x * m_neighbourVect.x + m_neighbourVect.y * m_neighbourVect.y + m_neighbourVect.z * m_neighbourVect.z;

    if(distanceSquared < m_distanceKnnSquared)
    {
        this->addNeighbour(&newNeighbour);
    }
    if(distanceSquared < newNeighbour.m_distanceKnnSquared)
    {
        newNeighbour.addNeighbour(this);
    }
}

void CloudVertexComputationStructure::computeTangentPlane()
{
    /*compute centroid (plane origin) as barycenters of neighbourhood*/
    m_tangentPlaneOrigin = DirectX::XMFLOAT3{ 0.f, 0.f, 0.f };

    if(!m_neighbours.empty())
    {
        auto endIter = m_neighbours.end();
        for(auto iter = m_neighbours.begin(); iter != endIter; ++iter)
        {
            auto& current = (*iter)->m_position;
            m_tangentPlaneOrigin.x += current.x;
            m_tangentPlaneOrigin.y += current.y;
            m_tangentPlaneOrigin.z += current.z;
        }

        float neighbourCountF = static_cast<float>(m_neighbours.size());

        m_tangentPlaneOrigin.x /= neighbourCountF;
        m_tangentPlaneOrigin.y /= neighbourCountF;
        m_tangentPlaneOrigin.z /= neighbourCountF;

        /*compute normal of tangent plane*/

        //compute covariance matrix
        float neighbourhoodCovarianceMatrix[9];
        this->computeAverage3DCovarianceMatrixFromSetsPointsForNeighbourhood(neighbourhoodCovarianceMatrix);

        Slg3DScanner::computeEigenValueAndEigenVectorsFromCovarianceMatrixAndOutputNormal(neighbourhoodCovarianceMatrix, m_tangentPlaneNormal);
    }
    else
    {
        m_tangentPlaneNormal = DirectX::XMFLOAT3{ 0.f, 0.f, 0.f };
    }
}

void CloudVertexComputationStructure::computeAverage3DCovarianceMatrixFromSetsPointsForNeighbourhood(float(&outResult)[9])
{
    memset(outResult, 0, sizeof(outResult));

    float tempCovariance[6];

    auto endIter = m_neighbours.end();
    for(auto iter = m_neighbours.begin(); iter != endIter; ++iter)
    {
        Slg3DScanner::compute3DCovarianceMatrixMinimal((*iter)->m_position, m_tangentPlaneOrigin, tempCovariance);

        outResult[0] += tempCovariance[0];
        outResult[1] += tempCovariance[1];
        outResult[2] += tempCovariance[2];
        outResult[4] += tempCovariance[3];
        outResult[5] += tempCovariance[4];
        outResult[8] += tempCovariance[5];
    }

    float neighbourCountF = static_cast<float>(m_neighbours.size());

    outResult[0] /= neighbourCountF;
    outResult[1] /= neighbourCountF;
    outResult[2] /= neighbourCountF;
    outResult[4] /= neighbourCountF;
    outResult[5] /= neighbourCountF;
    outResult[8] /= neighbourCountF;

    //covariance matrix are always symetric
    outResult[3] = outResult[1];
    outResult[6] = outResult[2];
    outResult[7] = outResult[5];
}