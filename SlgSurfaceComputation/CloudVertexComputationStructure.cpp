#include "SlgSurfaceComputationPCH.h"
#include "CloudVertexComputationStructure.h"

#include "InputCloudVertex.h"

#include "SlgMath.h"

using namespace Slg3DScanner;


CloudVertexComputationStructure::CloudVertexComputationStructure(const InputCloudVertex& inputVertex) :
    m_position{ inputVertex.m_vertex },
    m_distanceKnnSquared{ CloudVertexComputationStructure::DEFAULT_KNN_DISTANCE_SQUARED },
    m_mutex{ std::make_shared<std::mutex>() }
{
    m_neighbours.reserve(128);
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
    m_tangentPlaneOrigin{ other.m_tangentPlaneOrigin }
{
    std::swap(other.m_mutex, m_mutex);
    std::swap(other.m_neighbours, m_neighbours);
}

CloudVertexComputationStructure::~CloudVertexComputationStructure()
{
}

void CloudVertexComputationStructure::swap(CloudVertexComputationStructure& other)
{
    std::swap(m_distanceKnnSquared, other.m_distanceKnnSquared);
    std::swap(m_tangentPlaneNormal, other.m_tangentPlaneNormal);
    std::swap(m_tangentPlaneOrigin, other.m_tangentPlaneOrigin);
    std::swap(other.m_neighbours, m_neighbours);
    std::swap(other.m_mutex, m_mutex);
}

void CloudVertexComputationStructure::addNeighbour(CloudVertexComputationStructure* neighbour)
{
    std::lock_guard<std::mutex> autoLocker{ *m_mutex };
    m_neighbours.emplace_back(neighbour);
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