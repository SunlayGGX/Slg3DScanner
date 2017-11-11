#include "SlgSurfaceComputationPCH.h"
#include "DelaunayComputator.h"

using namespace Slg3DScanner;

DelaunayComputator::DelaunayComputator(std::vector<CloudVertexComputationStructureSimple>& pointList) :
    m_pointList{ pointList }
{
}

DelaunayComputator::~DelaunayComputator()
{
}

const std::vector<CloudVertexComputationStructureSimple>& DelaunayComputator::getPointList() const noexcept
{
    return m_pointList;
}

const std::list<DelaunayTriangle>& DelaunayComputator::getTriangleList() const noexcept
{
    return m_indexList;
}
