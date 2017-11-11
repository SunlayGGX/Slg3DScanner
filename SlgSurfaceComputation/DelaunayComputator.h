#pragma once

#include "DelaunayStructure.h"


namespace Slg3DScanner
{
    class CloudVertexComputationStructureSimple;
    
    class DelaunayComputator
    {
    public:
        struct DivideAndConquer {};
        struct SimpleTriangulation {};

    private:
        std::vector<CloudVertexComputationStructureSimple>& m_pointList;
        std::list<DelaunayTriangle> m_indexList;

    public:
        DelaunayComputator(std::vector<CloudVertexComputationStructureSimple>& pointList);
        ~DelaunayComputator();

        const std::vector<CloudVertexComputationStructureSimple>& getPointList() const noexcept;
        const std::list<DelaunayTriangle>& getTriangleList() const noexcept;

    private:
        /*Divide and conquer "pseudo" delaunay triangulation*/
        void compute(DivideAndConquer);

        void delaunaySort();
        void mergeDelaunay();
        void mergeDelaunayRecursive(DelaunayTriangle::IndexType beginIndex, DelaunayTriangle::IndexType endIndex, std::list<DelaunayTriangle>& outMergeContainer);
        void mergeSubsetsPointsIntoOne(std::list<DelaunayTriangle>& inContainer1, std::list<DelaunayTriangle>& inContainer2, std::list<DelaunayTriangle>& outMergeContainer);

        DelaunayTriangle::IndexType findBottomPointIndex(const std::list<DelaunayTriangle>& inContainer, bool isLeftContainer) const;
        DelaunayTriangle::IndexType findCandidatePoint(const DelaunayEdge& baseEdge, std::list<DelaunayTriangle>& inOutContainer, const bool isLeftContainer) const;

        bool isInsideCircumcenterCircle(DelaunayTriangle::IndexType pointIndex, const DelaunayTriangle& triangleCheck) const;

        /*Simple triangulation based on plane triangulation resolution (not delaunay)*/
        void compute(SimpleTriangulation);

        void scanSort();
        void triangulate();
        DelaunayTriangle::IndexType searchUpNeighbour(DelaunayTriangle::IndexType pointIndex) const;

    public:
        template<class DelaunayAlgoType>
        void compute()
        {
            if(m_pointList.size() > 2)
            {
                this->compute(DelaunayAlgoType{});
            }
        }
    };
}
