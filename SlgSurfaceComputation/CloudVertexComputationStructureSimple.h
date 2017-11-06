#pragma once

namespace Slg3DScanner
{
    struct InputCloudVertex;
    class ComputationCloudStructureSimple;

    class CloudVertexComputationStructureSimple
    {
    private:
        friend ComputationCloudStructureSimple;

    private:
        DirectX::XMFLOAT3 m_position;

        DirectX::XMFLOAT2 m_projectedPosition;

    public:
        CloudVertexComputationStructureSimple(const InputCloudVertex& inputPoint);
        CloudVertexComputationStructureSimple(CloudVertexComputationStructureSimple&);
        CloudVertexComputationStructureSimple(CloudVertexComputationStructureSimple&&);
        ~CloudVertexComputationStructureSimple();

        CloudVertexComputationStructureSimple& operator=(const CloudVertexComputationStructureSimple&);
        CloudVertexComputationStructureSimple& operator=(CloudVertexComputationStructureSimple&& other);

    public:
        void swap(CloudVertexComputationStructureSimple& other);
    };
}
