#include "SlgSurfaceComputationPCH.h"
#include "CloudVertexComputationStructureSimple.h"

#include "InputCloudVertex.h"

#include "ComputationCloudStructureSimple.h"

using namespace Slg3DScanner;

CloudVertexComputationStructureSimple::CloudVertexComputationStructureSimple(const InputCloudVertex& inputVertex) :
    m_position{ inputVertex.m_vertex }
{
}

CloudVertexComputationStructureSimple::CloudVertexComputationStructureSimple(CloudVertexComputationStructureSimple&) = default;
CloudVertexComputationStructureSimple& CloudVertexComputationStructureSimple::operator=(const CloudVertexComputationStructureSimple&) = default;

CloudVertexComputationStructureSimple& CloudVertexComputationStructureSimple::operator=(CloudVertexComputationStructureSimple&& other)
{
    this->swap(CloudVertexComputationStructureSimple{ std::forward<CloudVertexComputationStructureSimple>(other) });
    return *this;
}

CloudVertexComputationStructureSimple::CloudVertexComputationStructureSimple(CloudVertexComputationStructureSimple&& other) :
    m_position{ other.m_position },
    m_projectedPosition{ other.m_projectedPosition }
{

}

CloudVertexComputationStructureSimple::~CloudVertexComputationStructureSimple()
{
}

void CloudVertexComputationStructureSimple::swap(CloudVertexComputationStructureSimple& other)
{
    std::swap(other.m_position, m_position);
    std::swap(other.m_projectedPosition, m_projectedPosition);
}
