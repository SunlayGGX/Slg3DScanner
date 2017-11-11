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

        const DirectX::XMFLOAT2& getProjectedPosition() const;
        const DirectX::XMFLOAT3& get3DPosition() const;

    public:
        void projectOnPlane(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& rightAxis, const DirectX::XMFLOAT3& upAxis);

        void swap(CloudVertexComputationStructureSimple& other);
    };
}
