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
        {}

        ~CloudVertex() = default;

    public:
        FORCEINLINE std::string prettyPrintObjStream() const
        {
            return "v " + std::to_string(m_vertex.x) + ' ' + std::to_string(m_vertex.y) + ' ' + std::to_string(m_vertex.z) + '\n';
        }
    };

    struct InternalCloudMesh
    {
    public:
        std::vector<CloudVertex> m_vertexes;
        std::vector<unsigned int> m_indexes;

    public:
        InternalCloudMesh() = default;
        InternalCloudMesh(const InternalCloudMesh&) = default;
        InternalCloudMesh& operator=(const InternalCloudMesh&) = default;

        InternalCloudMesh(InternalCloudMesh&& other) :
            m_vertexes{ std::move(other.m_vertexes) },
            m_indexes{ std::move(other.m_indexes) }
        {}

        InternalCloudMesh& operator=(InternalCloudMesh&& other)
        {
            m_vertexes = std::move(other.m_vertexes);
            m_indexes = std::move(other.m_indexes);
            return *this;
        }

        bool isEmpty() const
        {
            return m_vertexes.empty();
        }
    };
}
