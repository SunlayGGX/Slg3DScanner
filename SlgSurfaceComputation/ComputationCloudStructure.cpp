#include "SlgSurfaceComputationPCH.h"
#include "ComputationCloudStructure.h"

#include "CloudVertex.h"
#include "InputCloudVertex.h"

#include <future>
#include <algorithm>

using namespace Slg3DScanner;


ComputationCloudBox::ComputationCloudBox(const float(&boxLimits)[ComputationCloudBox::LIMIT_COUNT])
{
    memcpy(m_boxLimits, boxLimits, ComputationCloudBox::LIMIT_COUNT * sizeof(float));
    m_connectedNeighbour.reserve(26);
}

ComputationCloudBox::~ComputationCloudBox() = default;

bool ComputationCloudBox::isInsideBox(const DirectX::XMFLOAT3& pointToCheck) const
{
    return
        m_boxLimits[ComputationCloudBox::X_MIN] < pointToCheck.x && m_boxLimits[ComputationCloudBox::X_MAX] > pointToCheck.x &&
        m_boxLimits[ComputationCloudBox::Y_MIN] < pointToCheck.y && m_boxLimits[ComputationCloudBox::Y_MAX] > pointToCheck.y &&
        m_boxLimits[ComputationCloudBox::Z_MIN] < pointToCheck.z && m_boxLimits[ComputationCloudBox::Z_MAX] > pointToCheck.z;
}

FORCEINLINE bool ComputationCloudBox::isNearPoint(const unsigned int xEdgeIndex, const unsigned int yEdgeIndex, const unsigned int zEdgeIndex, const DirectX::XMFLOAT3& pointToCheck, float distanceSquaredToConsiderNear) const
{
    float xDelta = pointToCheck.x - m_boxLimits[xEdgeIndex];
    float yDelta = pointToCheck.y - m_boxLimits[yEdgeIndex];
    float zDelta = pointToCheck.z - m_boxLimits[zEdgeIndex];
    
    return (xDelta * xDelta + yDelta * yDelta + zDelta * zDelta) < distanceSquaredToConsiderNear;
}

void ComputationCloudBox::optimize(std::size_t preallocatedPointCount)
{
    //shrink to fit memory taken by neighbours
    m_connectedNeighbour.shrink_to_fit();

    //sort for cache efficiency
    std::sort(m_connectedNeighbour.begin(), m_connectedNeighbour.end());

    //reserve a certain count of vertexes
    m_cloudVertexInside.reserve(preallocatedPointCount);
}

bool ComputationCloudBox::isNear(const DirectX::XMFLOAT3& pointToCheck, float distanceSquaredToConsiderNear) const
{
    return
        this->isNearPoint(ComputationCloudBox::X_MIN, ComputationCloudBox::Y_MIN, ComputationCloudBox::Z_MIN, pointToCheck, distanceSquaredToConsiderNear) ||
        this->isNearPoint(ComputationCloudBox::X_MIN, ComputationCloudBox::Y_MAX, ComputationCloudBox::Z_MIN, pointToCheck, distanceSquaredToConsiderNear) ||
        this->isNearPoint(ComputationCloudBox::X_MAX, ComputationCloudBox::Y_MIN, ComputationCloudBox::Z_MIN, pointToCheck, distanceSquaredToConsiderNear) ||
        this->isNearPoint(ComputationCloudBox::X_MAX, ComputationCloudBox::Y_MAX, ComputationCloudBox::Z_MIN, pointToCheck, distanceSquaredToConsiderNear) ||
        this->isNearPoint(ComputationCloudBox::X_MIN, ComputationCloudBox::Y_MIN, ComputationCloudBox::Z_MAX, pointToCheck, distanceSquaredToConsiderNear) ||
        this->isNearPoint(ComputationCloudBox::X_MIN, ComputationCloudBox::Y_MAX, ComputationCloudBox::Z_MAX, pointToCheck, distanceSquaredToConsiderNear) ||
        this->isNearPoint(ComputationCloudBox::X_MAX, ComputationCloudBox::Y_MIN, ComputationCloudBox::Z_MAX, pointToCheck, distanceSquaredToConsiderNear) ||
        this->isNearPoint(ComputationCloudBox::X_MAX, ComputationCloudBox::Y_MAX, ComputationCloudBox::Z_MAX, pointToCheck, distanceSquaredToConsiderNear);
}

void ComputationCloudBox::bindNeighbour(ComputationCloudBox* neighbour)
{
    this->m_connectedNeighbour.emplace_back(neighbour);
    neighbour->m_connectedNeighbour.emplace_back(this);
}

ComputationCloudStructure::ComputationCloudStructure(const InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount) :
    m_vertexCount{ inputedVertexCount }
{
    this->generateBoxArray(inInputedVertexes);
    this->fillBoxArray(inInputedVertexes);
    this->internalOptimize();
}

ComputationCloudStructure::~ComputationCloudStructure() = default;

void ComputationCloudStructure::generateBoxArray(const InputCloudVertex* inInputedVertexes)
{
    //find the limits of the box containing all the mesh cloud
    float bigBoxLimits[ComputationCloudBox::LIMIT_COUNT] = 
    { 
        std::numeric_limits<float>::max(), std::numeric_limits<float>::min(),
        std::numeric_limits<float>::max(), std::numeric_limits<float>::min(),
        std::numeric_limits<float>::max(), std::numeric_limits<float>::min()
    };

    {
        const InputCloudVertex* endIter = inInputedVertexes + m_vertexCount;
        for(const InputCloudVertex* iter = inInputedVertexes; iter != endIter; ++iter)
        {
            if(bigBoxLimits[ComputationCloudBox::X_MIN] > iter->m_vertex.x)
            {
                bigBoxLimits[ComputationCloudBox::X_MIN] = iter->m_vertex.x;
            }
            if(bigBoxLimits[ComputationCloudBox::X_MAX] < iter->m_vertex.x)
            {
                bigBoxLimits[ComputationCloudBox::X_MAX] = iter->m_vertex.x;
            }
            if(bigBoxLimits[ComputationCloudBox::Y_MIN] > iter->m_vertex.y)
            {
                bigBoxLimits[ComputationCloudBox::Y_MIN] = iter->m_vertex.y;
            }
            if(bigBoxLimits[ComputationCloudBox::Y_MAX] < iter->m_vertex.y)
            {
                bigBoxLimits[ComputationCloudBox::Y_MAX] = iter->m_vertex.y;
            }
            if(bigBoxLimits[ComputationCloudBox::Z_MIN] > iter->m_vertex.z)
            {
                bigBoxLimits[ComputationCloudBox::Z_MIN] = iter->m_vertex.z;
            }
            if(bigBoxLimits[ComputationCloudBox::Z_MAX] < iter->m_vertex.z)
            {
                bigBoxLimits[ComputationCloudBox::Z_MAX] = iter->m_vertex.z;
            }
        }
    }

    //get the length of the big box edge
    float bigBoxXDist = bigBoxLimits[ComputationCloudBox::X_MAX] - bigBoxLimits[ComputationCloudBox::X_MIN];
    float bigBoxYDist = bigBoxLimits[ComputationCloudBox::Y_MAX] - bigBoxLimits[ComputationCloudBox::Y_MIN];
    float bigBoxZDist = bigBoxLimits[ComputationCloudBox::Z_MAX] - bigBoxLimits[ComputationCloudBox::Z_MIN];

    //increase length by incPercent% on each side
    constexpr const float incPercent = 0.1f;
    bigBoxLimits[ComputationCloudBox::X_MIN] -= incPercent * bigBoxXDist;
    bigBoxLimits[ComputationCloudBox::X_MAX] += incPercent * bigBoxXDist;
    bigBoxLimits[ComputationCloudBox::Y_MIN] -= incPercent * bigBoxYDist;
    bigBoxLimits[ComputationCloudBox::Y_MAX] += incPercent * bigBoxYDist;
    bigBoxLimits[ComputationCloudBox::Z_MIN] -= incPercent * bigBoxZDist;
    bigBoxLimits[ComputationCloudBox::Z_MAX] += incPercent * bigBoxZDist;

    //recompute the real length
    bigBoxXDist = bigBoxLimits[ComputationCloudBox::X_MAX] - bigBoxLimits[ComputationCloudBox::X_MIN];
    bigBoxYDist = bigBoxLimits[ComputationCloudBox::Y_MAX] - bigBoxLimits[ComputationCloudBox::Y_MIN];
    bigBoxZDist = bigBoxLimits[ComputationCloudBox::Z_MAX] - bigBoxLimits[ComputationCloudBox::Z_MIN];

    //compute box counts on the edge
    std::size_t m_xBoxCount = std::min(static_cast<std::size_t>(ComputationCloudStructure::MAX_BOX_COUNT_ON_X_AXIS), static_cast<std::size_t>(bigBoxXDist / ComputationCloudStructure::MIN_BOX_LENGTH));
    std::size_t m_yBoxCount = std::min(static_cast<std::size_t>(ComputationCloudStructure::MAX_BOX_COUNT_ON_Y_AXIS), static_cast<std::size_t>(bigBoxYDist / ComputationCloudStructure::MIN_BOX_LENGTH));
    std::size_t m_zBoxCount = std::min(static_cast<std::size_t>(ComputationCloudStructure::MAX_BOX_COUNT_ON_Z_AXIS), static_cast<std::size_t>(bigBoxZDist / ComputationCloudStructure::MIN_BOX_LENGTH));

    //compute steps to generate box array
    float stepX = bigBoxXDist / static_cast<float>(m_xBoxCount);
    float stepY = bigBoxYDist / static_cast<float>(m_yBoxCount);
    float stepZ = bigBoxZDist / static_cast<float>(m_zBoxCount);

    //generate the box vector
    float boxIncrementsLimits[ComputationCloudBox::LIMIT_COUNT];

    const std::size_t totalBoxCount = m_xBoxCount * m_yBoxCount * m_zBoxCount;
    m_boxArray.reserve(totalBoxCount);

    for(std::size_t z = 0; z < m_zBoxCount; ++z)
    {
        boxIncrementsLimits[ComputationCloudBox::Z_MIN] = bigBoxLimits[ComputationCloudBox::Z_MIN] + stepZ * static_cast<float>(z);
        boxIncrementsLimits[ComputationCloudBox::Z_MAX] = boxIncrementsLimits[ComputationCloudBox::Z_MIN] + stepZ;

        for(std::size_t y = 0; y < m_yBoxCount; ++y)
        {
            boxIncrementsLimits[ComputationCloudBox::Y_MIN] = bigBoxLimits[ComputationCloudBox::Y_MIN] + stepY * static_cast<float>(y);
            boxIncrementsLimits[ComputationCloudBox::Y_MAX] = boxIncrementsLimits[ComputationCloudBox::Y_MIN] + stepY;

            for(std::size_t x = 0; x < m_yBoxCount; ++x)
            {
                boxIncrementsLimits[ComputationCloudBox::X_MIN] = bigBoxLimits[ComputationCloudBox::X_MIN] + stepX * static_cast<float>(x);
                boxIncrementsLimits[ComputationCloudBox::X_MAX] = boxIncrementsLimits[ComputationCloudBox::X_MIN] + stepX;

                m_boxArray.emplace_back(boxIncrementsLimits);
            }
        }
    }

    //connect everything (all box) together
    
    //connect all box to boxes on the upper z level
    const std::size_t yLevelStep = m_xBoxCount;
    const std::size_t zLevelStep = m_xBoxCount * m_yBoxCount;
    const std::size_t lastElement = m_xBoxCount * m_yBoxCount * m_zBoxCount; //last element of last level
    const std::size_t lastXLevel = m_xBoxCount - 1; //last element of x level
    const std::size_t lastYLevel = m_yBoxCount - 1; //last element of y level
    const std::size_t lastZLevel = m_zBoxCount - 1; //last element of z level
    for(std::size_t index = 0; index < lastElement; ++index)
    {
        ComputationCloudBox* current = &m_boxArray[index];

        auto xLevel = index % m_xBoxCount;
        auto yLevel = (index / m_xBoxCount) % yLevelStep;
        auto zLevel = index / zLevelStep;

        //bind 6 faces
        if(xLevel != lastXLevel)
        {
            current->bindNeighbour(&m_boxArray[index + 1]);
        }

        if(yLevel != lastYLevel)
        {
            current->bindNeighbour(&m_boxArray[index + yLevelStep]);
        }

        if(zLevel != lastZLevel)
        {
            current->bindNeighbour(&m_boxArray[index + zLevelStep]);
        }

        //bind diagonals
        if(xLevel != lastXLevel)
        {
            std::size_t diagonalIndex = index + 1;

            //straight diagonals
            if(zLevel != 0)
            {
                current->bindNeighbour(&m_boxArray[diagonalIndex - zLevelStep]);
            }

            if(zLevel != lastZLevel)
            {
                current->bindNeighbour(&m_boxArray[diagonalIndex + zLevelStep]);
            }

            if(yLevel != 0)
            {
                current->bindNeighbour(&m_boxArray[diagonalIndex - yLevelStep]);
            }

            if(yLevel != lastYLevel)
            {
                current->bindNeighbour(&m_boxArray[diagonalIndex + yLevelStep]);
            }

            //diago-diagonals
            if(yLevel != 0 && zLevel != 0)
            {
                current->bindNeighbour(&m_boxArray[diagonalIndex - zLevelStep - yLevelStep]);
            }

            if(yLevel != lastYLevel && zLevel != 0)
            {
                current->bindNeighbour(&m_boxArray[diagonalIndex - zLevelStep + yLevelStep]);
            }

            if(yLevel != lastYLevel && zLevel != lastZLevel)
            {
                current->bindNeighbour(&m_boxArray[diagonalIndex + zLevelStep + yLevelStep]);
            }

            if(yLevel != 0 && zLevel != lastZLevel)
            {
                current->bindNeighbour(&m_boxArray[diagonalIndex + zLevelStep - yLevelStep]);
            }
        }

        //side straight diago
        if(yLevel != lastYLevel)
        {
            if(zLevel != 0)
            {
                current->bindNeighbour(&m_boxArray[index - zLevelStep + yLevelStep]);
            }

            if(zLevel != lastZLevel)
            {
                current->bindNeighbour(&m_boxArray[index + zLevelStep + yLevelStep]);
            }
        }
    }

    //for efficiency
    std::size_t preallocatedSize = (m_vertexCount / totalBoxCount) + 1; //maybe, point cloud is homogeneous... +1 is to round up (nothing to lose)

    const auto endIter = m_boxArray.end();
    for(auto iter = m_boxArray.begin(); iter != endIter; ++iter)
    {
        iter->optimize(preallocatedSize);
    }
}

void ComputationCloudStructure::fillBoxArray(const InputCloudVertex* inInputedVertexes)
{
    //Transfer input point to computationStructure
    bool infiniteLoopProtect = false;
    const auto endBoxArrayIter = m_boxArray.end();
    auto boxArrayIter = m_boxArray.begin();
    for(std::size_t iter = 0; iter != m_vertexCount;)
    {
        auto& inputVertex = inInputedVertexes[iter];
        bool found = false;

        for(; boxArrayIter != endBoxArrayIter; ++boxArrayIter)
        {
            if(boxArrayIter->isInsideBox(inputVertex.m_vertex))
            {
                boxArrayIter->m_cloudVertexInside.emplace_back(inputVertex);
                ++iter;
                found = true;
                break;
            }
        }

        if(!found)
        {
            assert(!infiniteLoopProtect); //avoid to "not find" twice.
            infiniteLoopProtect = true;

            boxArrayIter = m_boxArray.begin();
        }
        else
        {
            infiniteLoopProtect = false;
        }
    }
}

void ComputationCloudStructure::transferToFinal(std::vector<CloudVertex>& outFinalPointCloudArray) const
{
    //clean the vector before use.
    outFinalPointCloudArray.clear();
    outFinalPointCloudArray.reserve(m_vertexCount);

    //Transfer computationStructure point to final point
    const auto endBoxArrayIter = m_boxArray.end();
    for(auto boxArrayIter = m_boxArray.begin(); boxArrayIter != endBoxArrayIter; ++boxArrayIter)
    {
        auto vertexEndIter = boxArrayIter->m_cloudVertexInside.end();
        for(auto vertexIter = boxArrayIter->m_cloudVertexInside.begin(); vertexIter != vertexEndIter; ++vertexIter)
        {
            outFinalPointCloudArray.emplace_back(vertexIter->m_position);
        }
    }
}

void ComputationCloudStructure::internalOptimize()
{
    /*
    Warning : MUST NOT do operation which could reallocate box array.
    */

    //disconnect vertex box that are empty
    auto endIter = m_boxArray.end();
    for(auto iter = m_boxArray.begin(); iter != endIter; ++iter)
    {
        auto& currentNeighbourArray = iter->m_connectedNeighbour;
        unsigned int removed = 0;
        std::stable_partition(
            currentNeighbourArray.begin(),
            currentNeighbourArray.end(),
            [&removed](const ComputationCloudBox* box)
        {
            if(box->m_cloudVertexInside.empty())
            {
                ++removed;
                return false;
            }
            
            return true;
        });
        
        while(removed != 0)
        {
            currentNeighbourArray.pop_back();
            --removed;
        }
    }
}

void ComputationCloudStructure::compute()
{
    if(m_vertexCount < 2)
    {
        return;
    }

    this->computeNeighbourhoods();
    this->computeTangentPlanes();
}

void ComputationCloudStructure::computeNeighbourhoods()
{
    std::vector<std::future<void>> asyncArray;
    asyncArray.reserve(m_boxArray.size());

    auto endIter = m_boxArray.end();
    for(auto iter = m_boxArray.begin(); iter != endIter; ++iter)
    {
        auto& cloudVertexInside = iter->m_cloudVertexInside;
        if(!cloudVertexInside.empty())
        {
            asyncArray.emplace_back(std::async([this, iter, &cloudVertexInside = iter->m_cloudVertexInside]() 
            {
                std::vector<ComputationCloudBox*> alreadyVisitedNeighborhood;
                alreadyVisitedNeighborhood.reserve(26);

                const auto endNeighbourIter = iter->m_connectedNeighbour.end();
                auto endJiter = cloudVertexInside.end();
                auto endKIter = endJiter - 1;
                for(auto kiter = cloudVertexInside.begin(); kiter != endKIter; ++kiter)
                {
                    for(auto jiter = kiter + 1; jiter != endJiter; ++jiter)
                    {
                        kiter->evaluateNeighbourhood(*jiter);
                    }

                    for(auto neighbourIter = iter->m_connectedNeighbour.begin(); neighbourIter != endNeighbourIter; ++neighbourIter)
                    {
                        this->internalEvaluateNeighbourhoodRecursive(*kiter, **neighbourIter, alreadyVisitedNeighborhood);
                    }
                }
            }));
        }
    }
}

void ComputationCloudStructure::internalEvaluateNeighbourhoodRecursive(CloudVertexComputationStructure& currentCloudVertex, ComputationCloudBox& boxNeighbour, std::vector<ComputationCloudBox*>& inOutAlreadyVisitedNeighborhood)
{
    if(boxNeighbour.m_cloudVertexInside.empty())
    {
        return;
    }

    if(std::find(inOutAlreadyVisitedNeighborhood.begin(), inOutAlreadyVisitedNeighborhood.end(), &boxNeighbour) == inOutAlreadyVisitedNeighborhood.end())
    {
        inOutAlreadyVisitedNeighborhood.emplace_back(&boxNeighbour);

        if(boxNeighbour.isNear(currentCloudVertex.m_position, currentCloudVertex.m_distanceKnnSquared))
        {
            auto endIter = boxNeighbour.m_cloudVertexInside.end();
            for(auto iter = boxNeighbour.m_cloudVertexInside.begin(); iter != endIter; ++iter)
            {
                currentCloudVertex.evaluateNeighbourhood(*iter);
            }

            auto endNeighbourIter = boxNeighbour.m_connectedNeighbour.end();
            for(auto neighbourIter = boxNeighbour.m_connectedNeighbour.begin(); neighbourIter != endNeighbourIter; ++neighbourIter)
            {
                this->internalEvaluateNeighbourhoodRecursive(currentCloudVertex, **neighbourIter, inOutAlreadyVisitedNeighborhood);
            }
        }
    }
}

void ComputationCloudStructure::computeTangentPlanes()
{
    auto endIter = m_boxArray.end();
    for(auto iter = m_boxArray.begin(); iter != endIter; ++iter)
    {
        if(!iter->m_cloudVertexInside.empty())
        {
            std::for_each(
                iter->m_cloudVertexInside.begin(), 
                iter->m_cloudVertexInside.end(), 
                [](CloudVertexComputationStructure& cloudVertex) 
            {
                cloudVertex.computeTangentPlane();
            });
        }
    }
}
