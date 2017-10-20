#include "IMesh.h"

#include "DirectXUtilitary.h"

using namespace Slg3DScanner;


IMesh::IMesh(const IMeshInitializer& other) :
    NamedObject{ other.name },
    m_meshParams{other.meshParameters },
    m_vertexBuffer{ nullptr },
    m_indexBuffer{ nullptr }
{

}

IMesh::~IMesh()
{
    DXRelease(
        m_vertexBuffer,
        m_indexBuffer
    );
}

PreInitializeCBufferParameterFromMeshInstance& IMesh::getMeshParams() noexcept
{
    return m_meshParams;
}

void IMesh::setBuffers(ID3D11DeviceContext* immediateContext, UINT stride, UINT offset)
{
    immediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    immediateContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
}

void IMesh::registerComponent()
{

}

void IMesh::unregisterComponent()
{

}

void IMesh::destroy()
{

}

void IMesh::update(float deltaTime)
{

}
