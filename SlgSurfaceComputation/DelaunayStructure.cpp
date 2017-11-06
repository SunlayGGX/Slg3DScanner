#include "DelaunayStructure.h"

#include "CloudVertexComputationStructureSimple.h"

using namespace Slg3DScanner;


DelaunayTriangle::DelaunayTriangle(std::size_t id0) :
    index0{ id0 }
{

}

DelaunayTriangle::DelaunayTriangle(std::size_t id0, std::size_t id1) :
    index0{ id0 },
    index1{ id1 }
{

}

DelaunayTriangle::DelaunayTriangle(std::size_t id0, std::size_t id1, std::size_t id2) :
    index0{ id0 },
    index1{ id1 },
    index2{ id2 }
{

}

float DelaunayTriangle::computeArea(const std::vector<CloudVertexComputationStructureSimple>& pointList) const
{
    return DelaunayTriangle::computeArea(pointList[index0].getProjectedPosition(), pointList[index1].getProjectedPosition(), pointList[index2].getProjectedPosition());
}

float DelaunayTriangle::computeArea(const DirectX::XMFLOAT2& A, const DirectX::XMFLOAT2& B, const DirectX::XMFLOAT2& C)
{
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

bool DelaunayTriangle::isCounterClockwise(const std::vector<CloudVertexComputationStructureSimple>& pointList, float& outArea) const
{
    outArea = this->computeArea(pointList);
    return outArea > 0.f;
}
