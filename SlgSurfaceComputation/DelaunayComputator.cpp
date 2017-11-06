#include "SlgSurfaceComputationPCH.h"
#include "DelaunayComputator.h"

#include <algorithm>

using namespace Slg3DScanner;

DelaunayComputator::DelaunayComputator(std::vector<DelaunayPoint2d>&& pointList) :
    m_pointList{ std::forward<std::vector<DelaunayPoint2d>>(pointList) }
{
}

DelaunayComputator::~DelaunayComputator()
{
}

const std::vector<DelaunayPoint2d>& DelaunayComputator::getPointList() const noexcept
{
    return m_pointList;
}

const std::vector<DelaunayTriangle>& DelaunayComputator::getTriangleList() const noexcept
{
    return m_indexList;
}

void DelaunayComputator::compute(DivideAndConquer)
{
    this->delaunaySort();
    this->mergeDelaunay();
}

void DelaunayComputator::delaunaySort()
{
    using CloudVertexType = decltype(m_pointList)::value_type;
    std::sort(m_pointList.begin(), m_pointList.end(), [](const CloudVertexType& cloudVertex1, const CloudVertexType& cloudVertex2) {
        if(cloudVertex1.x < cloudVertex2.x)
        {
            return true;
        }
        else if(cloudVertex1.x == cloudVertex2.x)
        {
            return cloudVertex1.y < cloudVertex2.y;
        }

        return false;
    });
}

void DelaunayComputator::mergeDelaunay()
{
    std::list<DelaunayTriangle> outList;
    this->mergeDelaunayRecursive(0, m_pointList.size() - 1, outList);

    m_indexList.clear();
    m_indexList.reserve(outList.size());
    std::copy(outList.begin(), outList.end(), std::back_inserter(m_indexList));
}

void DelaunayComputator::mergeDelaunayRecursive(std::size_t beginIndex, std::size_t endIndex, std::list<DelaunayTriangle>& outMergeContainer)
{
    std::size_t diff = endIndex - beginIndex + 1;
    if(diff > 3)
    {
        std::list<DelaunayTriangle> mergeContainer1;
        std::list<DelaunayTriangle> mergeContainer2;

        std::size_t half = beginIndex + (diff - 1) / 2;

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

std::size_t DelaunayComputator::findBottomPointIndex(const std::list<DelaunayTriangle>& inContainer, bool isLeftContainer) const
{
    float bottomValue = std::numeric_limits<float>::max();
    std::size_t result = 0;

    if(isLeftContainer)
    {
        float rightValue = std::numeric_limits<float>::min();

        auto endIter = inContainer.end();
        for(auto iter = inContainer.begin(); iter != endIter; ++iter)
        {
            const DelaunayPoint2d* pointToCheck = &m_pointList[iter->index0];
            if(pointToCheck->y < bottomValue || (pointToCheck->y == bottomValue && pointToCheck->x > rightValue))
            {
                result = iter->index0;
                bottomValue = pointToCheck->y;
                rightValue = pointToCheck->x;
            }

            if(iter->index1 != std::numeric_limits<std::size_t>::max())
            {
                pointToCheck = &m_pointList[iter->index1];
                if(pointToCheck->y < bottomValue || (pointToCheck->y == bottomValue && pointToCheck->x > rightValue))
                {
                    result = iter->index1;
                    bottomValue = pointToCheck->y;
                    rightValue = pointToCheck->x;
                }
            }

            if(iter->index2 != std::numeric_limits<std::size_t>::max())
            {
                pointToCheck = &m_pointList[iter->index2];
                if(pointToCheck->y < bottomValue || (pointToCheck->y == bottomValue && pointToCheck->x > rightValue))
                {
                    result = iter->index2;
                    bottomValue = pointToCheck->y;
                    rightValue = pointToCheck->x;
                }
            }
        }
    }
    else
    {
        float leftValue = std::numeric_limits<float>::max();

        auto endIter = inContainer.end();
        for(auto iter = inContainer.begin(); iter != endIter; ++iter)
        {
            const DelaunayPoint2d* pointToCheck = &m_pointList[iter->index0];
            if(pointToCheck->y < bottomValue || (pointToCheck->y == bottomValue && pointToCheck->x < leftValue))
            {
                result = iter->index0;
                bottomValue = pointToCheck->y;
                leftValue = pointToCheck->x;
            }

            if(iter->index1 != std::numeric_limits<std::size_t>::max())
            {
                pointToCheck = &m_pointList[iter->index1];
                if(pointToCheck->y < bottomValue || (pointToCheck->y == bottomValue && pointToCheck->x < leftValue))
                {
                    result = iter->index1;
                    bottomValue = pointToCheck->y;
                    leftValue = pointToCheck->x;
                }
            }

            if(iter->index2 != std::numeric_limits<std::size_t>::max())
            {
                pointToCheck = &m_pointList[iter->index2];
                if(pointToCheck->y < bottomValue || (pointToCheck->y == bottomValue && pointToCheck->x < leftValue))
                {
                    result = iter->index2;
                    bottomValue = pointToCheck->y;
                    leftValue = pointToCheck->x;
                }
            }
        }
    }

    return result;
}

bool DelaunayComputator::isInsideCircumcenterCircle(std::size_t pointIndex, const DelaunayTriangle& triangleCheck) const
{
    const DelaunayPoint2d& pointToCheck = m_pointList[pointIndex];
    const DelaunayPoint2d& pointA = m_pointList[triangleCheck.index0];
    const DelaunayPoint2d& pointB = m_pointList[triangleCheck.index1];
    const DelaunayPoint2d& pointC = m_pointList[triangleCheck.index2];

    return
        pointA.lengthSquared() * DelaunayTriangle::computeArea(pointB, pointC, pointToCheck) -
        pointB.lengthSquared() * DelaunayTriangle::computeArea(pointA, pointC, pointToCheck) +
        pointC.lengthSquared() * DelaunayTriangle::computeArea(pointA, pointB, pointToCheck) -
        pointToCheck.lengthSquared() * DelaunayTriangle::computeArea(pointA, pointB, pointC) > 0.f;
}

std::size_t DelaunayComputator::findCandidatePoint(const DelaunayEdge& baseEdge, std::list<DelaunayTriangle>& inOutContainer, const bool isLeftContainer) const
{
    std::list<std::size_t> indexCandidates;

    std::size_t baseIndex = isLeftContainer ? baseEdge.index0 : baseEdge.index1;

    //retrieve candidate triangles
    {
        auto endIter = inOutContainer.end();
        for(auto iter = inOutContainer.begin(); iter != endIter; ++iter)
        {
            std::size_t base;
            std::size_t indexA;
            std::size_t indexB;

            if(iter->index0 == baseIndex)
            {
                base = iter->index0;
                indexA = iter->index1;
                indexB = iter->index2;
            }
            else if(iter->index1 == baseIndex)
            {
                indexA = iter->index0;
                base = iter->index1;
                indexB = iter->index2;
            }
            else if(iter->index2 == baseIndex)
            {
                indexA = iter->index0;
                indexB = iter->index1;
                base = iter->index2;
            }
            else
            {
                continue;
            }

            if(indexA != std::numeric_limits<std::size_t>::max() && m_pointList[indexA].y > m_pointList[base].y && std::find(indexCandidates.begin(), indexCandidates.end(), indexA) == indexCandidates.end())
            {
                indexCandidates.emplace_back(indexA);
            }

            if(indexB != std::numeric_limits<std::size_t>::max() && m_pointList[indexB].y > m_pointList[base].y && std::find(indexCandidates.begin(), indexCandidates.end(), indexB) == indexCandidates.end())
            {
                indexCandidates.emplace_back(indexB);
            }
        }

        if(indexCandidates.empty())
        {
            return std::numeric_limits<std::size_t>::max();
        }
    }

    DelaunayPoint2d vectBaseEdge;
    if(isLeftContainer)
    {
        vectBaseEdge.x = m_pointList[baseEdge.index1].x - m_pointList[baseEdge.index0].x;
        vectBaseEdge.y = m_pointList[baseEdge.index1].y - m_pointList[baseEdge.index0].y;
    }
    else
    {
        vectBaseEdge.x = m_pointList[baseEdge.index0].x - m_pointList[baseEdge.index1].x;
        vectBaseEdge.y = m_pointList[baseEdge.index0].y - m_pointList[baseEdge.index1].y;
    }
    vectBaseEdge.normalize();

    const DelaunayPoint2d& basePoint = m_pointList[baseIndex];

    indexCandidates.sort(
        [&pointList = this->m_pointList, &baseIndex, &vectBaseEdge, &basePoint](std::size_t id1, std::size_t id2)
    {
        const DelaunayPoint2d& point1 = pointList[id1];
        DelaunayPoint2d candidateEdgeVect1{ point1.x - basePoint.x, point1.y - basePoint.y };
        candidateEdgeVect1.normalize();

        const DelaunayPoint2d& point2 = pointList[id2];
        DelaunayPoint2d candidateEdgeVect2{ point2.x - basePoint.x, point2.y - basePoint.y };
        candidateEdgeVect2.normalize();

        return DelaunayPoint2d::scalar(candidateEdgeVect1, vectBaseEdge) > DelaunayPoint2d::scalar(candidateEdgeVect2, vectBaseEdge);
    });

    auto lastCandidateIter = indexCandidates.end(); --lastCandidateIter;
    auto candidateIter = indexCandidates.begin();
    DelaunayTriangle preConstructed{ baseEdge.index0, baseEdge.index1 };

    //evaluate the candidate point
    for(; candidateIter != lastCandidateIter; ++candidateIter)
    {
        preConstructed.index2 = *candidateIter;
        auto nextCandidate = candidateIter; ++nextCandidate;
        if(!this->isInsideCircumcenterCircle(*nextCandidate, preConstructed))
        {
            break;
        }
        else
        {
            DelaunayTriangle toErase{ baseIndex, *candidateIter, *nextCandidate };
            auto found = std::find_if(
                inOutContainer.begin(),
                inOutContainer.end(),
                [&toErase](const DelaunayTriangle& triangle) {
                return
                    (toErase.index0 == triangle.index0 || toErase.index0 == triangle.index1 || toErase.index0 == triangle.index2) &&
                    (toErase.index1 == triangle.index0 || toErase.index1 == triangle.index1 || toErase.index1 == triangle.index2) &&
                    (toErase.index2 == triangle.index0 || toErase.index2 == triangle.index1 || toErase.index2 == triangle.index2);
            });

            if(found != inOutContainer.end())
            {
                inOutContainer.erase(found);
            }
        }
    }

    return *candidateIter;
}

void DelaunayComputator::mergeSubsetsPointsIntoOne(std::list<DelaunayTriangle>& inContainer1, std::list<DelaunayTriangle>& inContainer2, std::list<DelaunayTriangle>& outMergeContainer)
{
    if(inContainer1.empty() || inContainer2.empty())
    {
        return;
    }

    std::list<DelaunayTriangle> zip;

    DelaunayEdge baseEdge{ this->findBottomPointIndex(inContainer1, true), this->findBottomPointIndex(inContainer2, false) };

    for(;;)
    {
        DelaunayEdge candidateEdgeQuad{ this->findCandidatePoint(baseEdge, inContainer1, true), this->findCandidatePoint(baseEdge, inContainer2, false) };

        if(!(candidateEdgeQuad.index0 == std::numeric_limits<std::size_t>::max() || candidateEdgeQuad.index1 == std::numeric_limits<std::size_t>::max()))
        {
            /*
            this is not from original algorithm found in
            http://www.geom.uiuc.edu/~samuelp/del_project.html

            The reason is, I don't mind if it is not a real delaunay... I just want triangles to construct meshes, I'm not working on an optimal mesh... just a simpler mesh.
            */

            DelaunayTriangle toAdd{ baseEdge.index0, baseEdge.index1, candidateEdgeQuad.index0 };
            if(!this->isInsideCircumcenterCircle(candidateEdgeQuad.index1, toAdd))
            {
                zip.emplace_back(toAdd);
                toAdd.index0 = candidateEdgeQuad.index0;
                toAdd.index2 = candidateEdgeQuad.index1;
                zip.emplace_back(toAdd);
            }
            else
            {
                toAdd.index2 = candidateEdgeQuad.index1;
                zip.emplace_back(toAdd);
                toAdd.index1 = candidateEdgeQuad.index1;
                toAdd.index2 = candidateEdgeQuad.index0;
                zip.emplace_back(toAdd);
            }

            baseEdge = candidateEdgeQuad;
        }
        else
        {
            if(candidateEdgeQuad.index0 == std::numeric_limits<std::size_t>::max() && candidateEdgeQuad.index1 == std::numeric_limits<std::size_t>::max())
            {
                break;
            }

            if(candidateEdgeQuad.index0 == std::numeric_limits<std::size_t>::max())
            {
                zip.emplace_back(baseEdge.index0, baseEdge.index1, candidateEdgeQuad.index1);
                baseEdge.index1 = candidateEdgeQuad.index1;
            }
            else if(candidateEdgeQuad.index1 == std::numeric_limits<std::size_t>::max())
            {
                zip.emplace_back(baseEdge.index0, baseEdge.index1, candidateEdgeQuad.index0);
                baseEdge.index0 = candidateEdgeQuad.index0;
            }
        }
    }

    outMergeContainer.splice(outMergeContainer.end(), inContainer1);
    outMergeContainer.splice(outMergeContainer.end(), inContainer2);
    outMergeContainer.splice(outMergeContainer.end(), zip);

    //erase deprecated triangles
    for(auto iter = outMergeContainer.begin(); iter != outMergeContainer.end(); ++iter)
    {
        if(iter->index0 == std::numeric_limits<std::size_t>::max() || iter->index1 == std::numeric_limits<std::size_t>::max() || iter->index2 == std::numeric_limits<std::size_t>::max())
        {
            auto toErase = iter++;
            outMergeContainer.erase(toErase);
        }
    }
}
