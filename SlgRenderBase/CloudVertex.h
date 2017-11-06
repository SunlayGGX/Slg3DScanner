#pragma once


namespace Slg3DScanner
{
    struct CloudVertex
    {
    public:
        static constexpr const D3D11_INPUT_ELEMENT_DESC s_layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };


    public:
        DirectX::XMFLOAT3 m_vertex = { 0.f, 0.f, 0.f };


    public:
        constexpr CloudVertex() noexcept = default;
        constexpr CloudVertex(const CloudVertex&) noexcept = default;

        constexpr CloudVertex(float xPos, float yPos, float zPos) noexcept :
        m_vertex{ xPos, yPos, zPos }
        {}

        constexpr CloudVertex(const DirectX::XMFLOAT3& vertexPos) noexcept :
            m_vertex{ vertexPos }
        {
        }

        ~CloudVertex() = default;
    };

    struct InternalCloudMesh
    {
    public:
        std::vector<CloudVertex> m_vertexes;
        std::vector<std::size_t> m_indexes;
    };
}
