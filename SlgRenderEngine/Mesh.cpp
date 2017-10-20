#include "SlgRenderEnginePCH.h"
#include "Mesh.h"

#include "Material.h"

using namespace Slg3DScanner;

Mesh::Mesh(const MeshInitializer& meshInitializer) :
    NamedObject{ meshInitializer.name },
    IMesh{ meshInitializer }
{
}

Mesh::~Mesh() = default;

void Mesh::addMaterial(const MaterialInitializer& materialInit)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    m_materialArray.emplace_back(materialInit);
}

void Mesh::addMaterial(MaterialInitializer&& materialInit)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    m_materialArray.emplace_back(materialInit);
}
