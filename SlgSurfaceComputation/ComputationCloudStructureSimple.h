#pragma once

#include "CloudVertexComputationStructureSimple.h"
#include "DelaunayStructure.h"

namespace Slg3DScanner
{
    struct InputCloudVertex;
    struct InternalCloudMesh;

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

        void transferToFinal(InternalCloudMesh& outFinalPointCloudArray) const;

    private:
        void fillArray(const InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount);
        void initializeProjectivePlane(const DirectX::XMFLOAT3& scannerDir);

        void computeProjectionOnPlane();
        
        void computeDelaunay();
    };
}
