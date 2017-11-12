#include "SlgSurfaceComputationPCH.h"
#include "DelaunayComputator.h"

#include "CloudVertexComputationStructureSimple.h"

#include "SlgMath.h"

#include <algorithm>

//#define TEST
#include "InputCloudVertex.h"

using namespace Slg3DScanner;

namespace Slg3DScanner
{
    namespace DelaunayDetail
    {
        struct QuadHelper
        {
        public:
            DelaunayTriangle::IndexType m_topLeftIndex = std::numeric_limits<DelaunayTriangle::IndexType>::max();
            DelaunayTriangle::IndexType m_topRightIndex = std::numeric_limits<DelaunayTriangle::IndexType>::max();
            DelaunayTriangle::IndexType m_bottomLeftIndex = std::numeric_limits<DelaunayTriangle::IndexType>::max();
            DelaunayTriangle::IndexType m_bottomRightIndex = std::numeric_limits<DelaunayTriangle::IndexType>::max();

            const std::vector<CloudVertexComputationStructureSimple>& m_pointList;
            float m_thresholdDistance;


        public:
            QuadHelper(const std::vector<CloudVertexComputationStructureSimple>& ptList, float thresholdDistance) : 
                m_pointList{ ptList }, 
                m_thresholdDistance{ thresholdDistance }
            {}

            FORCEINLINE void reset()
            {
                m_topLeftIndex = std::numeric_limits<DelaunayTriangle::IndexType>::max();
                m_topRightIndex = std::numeric_limits<DelaunayTriangle::IndexType>::max();
                m_bottomLeftIndex = std::numeric_limits<DelaunayTriangle::IndexType>::max();
                m_bottomRightIndex = std::numeric_limits<DelaunayTriangle::IndexType>::max();
            }

            FORCEINLINE bool validate(const DelaunayTriangle::IndexType point)
            {
                return point != std::numeric_limits<DelaunayTriangle::IndexType>::max();
            }

            FORCEINLINE bool validateTriangle(const DirectX::XMFLOAT3& specificPoint)
            {
                return
                    Slg3DScanner::distanceSquaredBetween(specificPoint, m_pointList[m_topLeftIndex].get3DPosition()) < m_thresholdDistance &&
                    Slg3DScanner::distanceSquaredBetween(specificPoint, m_pointList[m_bottomRightIndex].get3DPosition()) < m_thresholdDistance;
            }

            FORCEINLINE bool validateUpTriangle()
            {           
                return this->validateTriangle(m_pointList[m_topRightIndex].get3DPosition());
            }

            FORCEINLINE bool validateDownTriangle()
            {
                return this->validateTriangle(m_pointList[m_bottomLeftIndex].get3DPosition());
            }

            FORCEINLINE void retrieveTriangles(DelaunayTriangle& outUpTriangle, bool& outUpValid, DelaunayTriangle& outDownTriangle, bool& outDownValid)
            {
                if(this->validate(m_topLeftIndex) && this->validate(m_bottomRightIndex))
                {
                    outUpValid = this->validate(m_topRightIndex) && this->validateUpTriangle();
                    outDownValid = this->validate(m_bottomLeftIndex) && this->validateDownTriangle();
                }
                else
                {
                    outUpValid = false;
                    outDownValid = false;
                    return;
                }

                if(outUpValid)
                {
                    outUpTriangle.index0 = m_bottomRightIndex;
                    outUpTriangle.index1 = m_topLeftIndex;
                    outUpTriangle.index2 = m_topRightIndex;
                }

                if(outDownValid)
                {
                    outDownTriangle.index0 = m_bottomLeftIndex;
                    outDownTriangle.index1 = m_topLeftIndex;
                    outDownTriangle.index2 = m_bottomRightIndex;
                }
            }
        };
    }
}

void DelaunayComputator::compute(SimpleTriangulation)
{
    this->scanSort();
    this->triangulate();
}

void DelaunayComputator::scanSort()
{
    using CloudVertexType = CloudVertexComputationStructureSimple;
    std::sort(m_pointList.begin(), m_pointList.end(), [](const CloudVertexType& cloudVertex1, const CloudVertexType& cloudVertex2) {
        if(cloudVertex1.getProjectedPosition().y < cloudVertex2.getProjectedPosition().y)
        {
            return true;
        }
        else if(cloudVertex1.getProjectedPosition().y == cloudVertex2.getProjectedPosition().y)
        {
            return cloudVertex1.getProjectedPosition().x < cloudVertex2.getProjectedPosition().x;
        }

        return false;
    });
}

DelaunayTriangle::IndexType DelaunayComputator::searchUpNeighbour(DelaunayTriangle::IndexType pointIndex) const
{
    DelaunayTriangle::IndexType result = std::numeric_limits<DelaunayTriangle::IndexType>::max();

    float minDist = std::numeric_limits<float>::max();

    const DirectX::XMFLOAT2& basePointPosition = m_pointList[pointIndex].getProjectedPosition();
    ++pointIndex;

    const auto endPointListIter = m_pointList.size();
    for(; pointIndex < endPointListIter; ++pointIndex)
    {
        const DirectX::XMFLOAT2& currentCheckedPointPosition = m_pointList[pointIndex].getProjectedPosition();
        if(currentCheckedPointPosition.y > basePointPosition.y)
        {
            float currentDist = Slg3DScanner::distanceSquaredBetween(basePointPosition, currentCheckedPointPosition);
            
            if(currentDist <= minDist)
            {
                minDist = currentDist;
                result = pointIndex;
            }
            else
            {
                break;
            }
        }
    }

    return result;
}

void DelaunayComputator::triangulate()
{
    m_indexList.clear();


#ifdef TEST
    /*
    */

    m_pointList.clear();

    InputCloudVertex test;
    test.m_vertex.z = 0.f;

    DirectX::XMFLOAT3 planeOrigin{ 0.f, 0.f, 0.f };
    DirectX::XMFLOAT3 planeRight{ -1.f, 0.f, 0.f };
    DirectX::XMFLOAT3 planeUp{ 0.f, 1.f, 0.f };

    constexpr const float advance = 2.f / 5.f;

    for(test.m_vertex.y = -1.f; test.m_vertex.y < 1.f; test.m_vertex.y += advance)
    {
        for(test.m_vertex.x = -1.f; test.m_vertex.x < 1.f; test.m_vertex.x += advance)
        {
            m_pointList.emplace_back(test);

            m_pointList.back().projectOnPlane(planeOrigin, planeRight, planeUp);
        }
    }

    /*
    */
#endif

    DelaunayDetail::QuadHelper quadHelp{ m_pointList, 0.00025f };

    DelaunayTriangle tmpUpTriangle{ 0, 0, 0 };
    bool tmpUpValid;

    DelaunayTriangle tmpDownTriangle{ 0, 0, 0 };
    bool tmpDownValid;

    const auto endPointListIter = m_pointList.size() - 1;
    for(auto iter = 0; iter != endPointListIter; ++iter)
    {
        quadHelp.m_bottomRightIndex = iter;
        quadHelp.m_bottomLeftIndex = iter + 1;

        //if(m_pointList[quadHelp.m_bottomRightIndex].getProjectedPosition().y == m_pointList[quadHelp.m_bottomLeftIndex].getProjectedPosition().y)
        {
            quadHelp.m_topRightIndex = this->searchUpNeighbour(quadHelp.m_bottomRightIndex);

            if(quadHelp.m_topRightIndex != quadHelp.m_bottomLeftIndex)
            {
                if(quadHelp.m_topRightIndex < endPointListIter)
                {
                    quadHelp.m_topLeftIndex = quadHelp.m_topRightIndex + 1;
                }
                else
                {
                    break;
                }

                //if(m_pointList[quadHelp.m_topRightIndex].getProjectedPosition().y == m_pointList[quadHelp.m_topLeftIndex].getProjectedPosition().y)
                {
                    quadHelp.retrieveTriangles(tmpUpTriangle, tmpUpValid, tmpDownTriangle, tmpDownValid);

                    if(tmpUpValid)
                    {
                        m_indexList.emplace_back(tmpUpTriangle);
                    }

                    if(tmpDownValid)
                    {
                        m_indexList.emplace_back(tmpDownTriangle);
                    }
                }
            }
            else
            {
                break;
            }
        }
    }
}
