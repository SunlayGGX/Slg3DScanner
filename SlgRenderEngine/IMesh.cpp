#include "IMesh.h"

#include "RenderSceneManager.h"

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

void IMesh::addMaterial(const MaterialInitializer& materialInit)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    m_materialArray.emplace_back(materialInit);
}

void IMesh::addMaterial(MaterialInitializer&& materialInit)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    m_materialArray.emplace_back(std::move(materialInit));
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
