#pragma once

namespace Slg3DScanner
{
    class CloudVertexComputationStructureSimple;

    struct DelaunayEdge
    {
        unsigned int index0;
        unsigned int index1;
    };

    struct DelaunayTriangle
    {
    public:
        using IndexType = unsigned int;

    public:
        IndexType index0 = std::numeric_limits<IndexType>::max();
        IndexType index1 = std::numeric_limits<IndexType>::max();
        IndexType index2 = std::numeric_limits<IndexType>::max();

    public:
        DelaunayTriangle(IndexType id0);
        DelaunayTriangle(IndexType id0, IndexType id1);
        DelaunayTriangle(IndexType id0, IndexType id1, IndexType id2);

    public:
        float computeArea(const std::vector<CloudVertexComputationStructureSimple>& pointList) const;
        bool isCounterClockwise(const std::vector<CloudVertexComputationStructureSimple>& pointList, float& outArea) const;

    public:
        static float computeArea(const DirectX::XMFLOAT2& A, const DirectX::XMFLOAT2& B, const DirectX::XMFLOAT2& C);
    };
}
