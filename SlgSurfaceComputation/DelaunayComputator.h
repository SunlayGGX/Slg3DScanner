#pragma once

#include "DelaunayStructure.h"


namespace Slg3DScanner
{
    class CloudVertexComputationStructureSimple;
    
    class DelaunayComputator
    {
    public:
        struct DivideAndConquer {};

    private:
        std::vector<CloudVertexComputationStructureSimple>& m_pointList;
        std::list<DelaunayTriangle> m_indexList;

    public:
        DelaunayComputator(std::vector<CloudVertexComputationStructureSimple>& pointList);
        ~DelaunayComputator();

        const std::vector<CloudVertexComputationStructureSimple>& getPointList() const noexcept;
        const std::list<DelaunayTriangle>& getTriangleList() const noexcept;

    private:
        void compute(DivideAndConquer);

        void delaunaySort();
        void mergeDelaunay();
        void mergeDelaunayRecursive(std::size_t beginIndex, std::size_t endIndex, std::list<DelaunayTriangle>& outMergeContainer);
        void mergeSubsetsPointsIntoOne(std::list<DelaunayTriangle>& inContainer1, std::list<DelaunayTriangle>& inContainer2, std::list<DelaunayTriangle>& outMergeContainer);

        std::size_t findBottomPointIndex(const std::list<DelaunayTriangle>& inContainer, bool isLeftContainer) const;
        std::size_t findCandidatePoint(const DelaunayEdge& baseEdge, std::list<DelaunayTriangle>& inOutContainer, const bool isLeftContainer) const;

        bool isInsideCircumcenterCircle(std::size_t pointIndex, const DelaunayTriangle& triangleCheck) const;

    public:
        template<class DelaunayAlgoType>
        void compute()
        {
            this->compute(DelaunayAlgoType{});
        }
    };
}
