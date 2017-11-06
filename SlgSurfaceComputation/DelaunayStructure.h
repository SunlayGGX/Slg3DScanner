#pragma once

namespace Slg3DScanner
{
    class CloudVertexComputationStructureSimple;

    struct DelaunayEdge
    {
        std::size_t index0;
        std::size_t index1;
    };

    struct DelaunayTriangle
    {
    public:
        std::size_t index0 = std::numeric_limits<std::size_t>::max();
        std::size_t index1 = std::numeric_limits<std::size_t>::max();
        std::size_t index2 = std::numeric_limits<std::size_t>::max();

    public:
        DelaunayTriangle(std::size_t id0);
        DelaunayTriangle(std::size_t id0, std::size_t id1);
        DelaunayTriangle(std::size_t id0, std::size_t id1, std::size_t id2);

    public:
        float computeArea(const std::vector<CloudVertexComputationStructureSimple>& pointList) const;
        bool isCounterClockwise(const std::vector<CloudVertexComputationStructureSimple>& pointList, float& outArea) const;

    public:
        static float computeArea(const DirectX::XMFLOAT2& A, const DirectX::XMFLOAT2& B, const DirectX::XMFLOAT2& C);
    };
}
