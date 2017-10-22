#pragma once


namespace Slg3DScanner
{
    struct InputCloudVertex
    {
    public:
        DirectX::XMFLOAT3 m_vertex = { 0.f, 0.f, 0.f };


    public:
        constexpr InputCloudVertex() noexcept = default;
        constexpr InputCloudVertex(const InputCloudVertex&) noexcept = default;

        constexpr InputCloudVertex(float xPos, float yPos, float zPos) noexcept :
            m_vertex{ xPos, yPos, zPos }
        {}

        constexpr InputCloudVertex(const DirectX::XMFLOAT3& vertexPos) noexcept :
            m_vertex{ vertexPos }
        {
        }

        ~InputCloudVertex() = default;
    };
}
