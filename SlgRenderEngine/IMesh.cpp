#include "IMesh.h"

#include "RenderSceneManager.h"

#include "DirectXUtilitary.h"

using namespace Slg3DScanner;


IMesh::IMesh(const IMeshInitializer& other) :
    NamedObject{ other.name },
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

void IMesh::destroy()
{

}
