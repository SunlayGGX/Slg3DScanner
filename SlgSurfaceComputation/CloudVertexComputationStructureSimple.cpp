#include "SlgSurfaceComputationPCH.h"
#include "CloudVertexComputationStructureSimple.h"

#include "InputCloudVertex.h"

#include "ComputationCloudStructureSimple.h"

#include "SlgMath.h"

using namespace Slg3DScanner;

CloudVertexComputationStructureSimple::CloudVertexComputationStructureSimple(const InputCloudVertex& inputVertex) :
    m_position{ inputVertex.m_vertex.x, inputVertex.m_vertex.y, inputVertex.m_vertex.z }
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

const DirectX::XMFLOAT2& CloudVertexComputationStructureSimple::getProjectedPosition() const
{
    return m_projectedPosition;
}

const DirectX::XMFLOAT3& CloudVertexComputationStructureSimple::get3DPosition() const
{
    return m_position;
}

void CloudVertexComputationStructureSimple::projectOnPlane(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& rightAxis, const DirectX::XMFLOAT3& upAxis)
{
    DirectX::XMFLOAT3 relativePosition{ m_position.x - origin.x, m_position.y - origin.y, m_position.z - origin.z };
    m_projectedPosition = { Slg3DScanner::scalar(relativePosition, rightAxis), Slg3DScanner::scalar(relativePosition, upAxis) };
}

void CloudVertexComputationStructureSimple::swap(CloudVertexComputationStructureSimple& other)
{
    std::swap(other.m_position, m_position);
    std::swap(other.m_projectedPosition, m_projectedPosition);
}
