#pragma once

namespace Slg3DScanner
{
    struct SimpleVertex
    {
    public:
        static constexpr const D3D11_INPUT_ELEMENT_DESC s_Layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };


    public:
        DirectX::XMFLOAT3 m_Vertex = { 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 m_Normal = { 0.f, 0.f, 1.f };
        DirectX::XMFLOAT2 m_UV = { 0.f, 0.f };


    public:
        constexpr SimpleVertex() noexcept = default;
        constexpr SimpleVertex(float xPos, float yPos, float zPos) noexcept :
            m_Vertex{ xPos, yPos, zPos }
        {
        }
        constexpr SimpleVertex(const DirectX::XMFLOAT3& vertexPos) noexcept :
            m_Vertex{ vertexPos }
        {
        }
        constexpr SimpleVertex(const DirectX::XMFLOAT3& vertexPos, const DirectX::XMFLOAT3& normal) noexcept :
            m_Vertex{ vertexPos },
            m_Normal{ normal }
        {
        }
        constexpr SimpleVertex(const DirectX::XMFLOAT3& vertexPos, const DirectX::XMFLOAT3& normal, const DirectX::XMFLOAT2& uv) noexcept :
            m_Vertex{ vertexPos },
            m_Normal{ normal },
            m_UV{ uv }
        {
        }

        ~SimpleVertex() = default;
    };


    struct TriangleIndex
    {
    public:
        UINT m_Index[3] = { 0, 0, 0 };

        constexpr TriangleIndex() noexcept = default;
        constexpr TriangleIndex(unsigned int i1, unsigned int i2, unsigned int i3) noexcept :
            m_Index{ i1, i2, i3 }
        {}

        ~TriangleIndex() = default;
    };


    template<unsigned int width, unsigned int height>
    struct IndexHelper
    {
        static_assert(width != 0, "ERROR : Please, provide a plane width > 0");
        static_assert(height != 0, "ERROR : Please, provide a plane height > 0");


    public:
        enum
        {
            PLANE_WIDTH = width,
            PLANE_HEIGHT = height,
            PLANE_WIDTH_POINT_COUNT = PLANE_WIDTH + 1,
            PLANE_HEIGHT_POINT_COUNT = PLANE_HEIGHT + 1,

            TRIANGLE_COUNT = PLANE_WIDTH * PLANE_HEIGHT * 2,

            INDEX_MEMORY_USAGE_IN_BYTE = TRIANGLE_COUNT * sizeof(TriangleIndex)
        };

        
    public:
        static inline TriangleIndex* attributeIndex(TriangleIndex* indexArray, unsigned int pointDownLeft, unsigned int pointUpLeft, unsigned int pointDownRight, unsigned int pointUpRight)
        {
            /*Up Triangle*/
            indexArray->m_Index[0] = pointDownLeft;
            indexArray->m_Index[1] = pointUpLeft;
            indexArray->m_Index[2] = pointUpRight;

            ++indexArray;


            /*Down Triangle*/
            indexArray->m_Index[0] = pointDownLeft;
            indexArray->m_Index[1] = pointUpRight;
            indexArray->m_Index[2] = pointDownRight;

            return indexArray + 1;
        }
    };
}


