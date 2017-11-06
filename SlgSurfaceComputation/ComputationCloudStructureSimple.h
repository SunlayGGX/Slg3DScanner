#pragma once

#include "CloudVertexComputationStructureSimple.h"
#include "DelaunayStructure.h"

namespace Slg3DScanner
{
    struct InputCloudVertex;
    struct CloudVertex;

    class ComputationCloudStructureSimple
    {
    private:
        std::vector<CloudVertexComputationStructureSimple> m_cloudArray;
        std::vector<DelaunayTriangle> m_indexTriangle;

        //plane
        DirectX::XMFLOAT3 m_projPlaneOrigin;
        DirectX::XMFLOAT3 m_projPlaneNormal;
        DirectX::XMFLOAT3 m_projPlaneRight;
        DirectX::XMFLOAT3 m_projPlaneUp;


    public:
        ComputationCloudStructureSimple(const DirectX::XMFLOAT3& scannerDir, const InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount);
        ~ComputationCloudStructureSimple();

    public:
        void compute();

        void transferToFinal(std::vector<CloudVertex>& outFinalPointCloudArray) const;

    private:
        void fillArray(const InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount);
        void initializeProjectivePlane(const DirectX::XMFLOAT3& scannerDir);

        void computeProjectionOnPlane();
        
        void computeDelaunayIndexes();
        void mergeDelaunay();
        void mergeDelaunayRecursive(std::size_t beginIndex, std::size_t endIndex, std::list<DelaunayTriangle>& outMergeContainer);
        void mergeSubsetsPointsIntoOne(std::list<DelaunayTriangle>& inContainer1, std::list<DelaunayTriangle>& inContainer2, std::list<DelaunayTriangle>& outMergeContainer);
    };
}
