#include "SlgSurfaceComputationPCH.h"
#include "PointCloudComputator.h"

#include "CloudVertexComputationStructure.h"

#include <future>

using namespace Slg3DScanner;

PointCloudComputator::PointCloudComputator(std::vector<CloudVertexComputationStructure>& workingCloud) :
    m_workingCloud{ &workingCloud }
{
}

PointCloudComputator::~PointCloudComputator()
{
}

void PointCloudComputator::setWorkingPointCloud(std::vector<CloudVertexComputationStructure>& workingCloud)
{
    m_workingCloud = &workingCloud;
}

void PointCloudComputator::computeNeighbourhoods()
{
    std::vector<std::future<void>> asyncComputer;
    asyncComputer.reserve(m_workingCloud->size() - 1);

    auto endJiter = m_workingCloud->end();
    auto endIter = endJiter - 1;
    for(auto iter = m_workingCloud->begin(); iter != endIter; ++iter)
    {
        asyncComputer.emplace_back(std::async(
            [iter, endJiter]()
        {
            for(auto jiter = iter + 1; jiter != endJiter; ++jiter)
            {
                iter->evaluateNeighbourhood(*jiter);
            }
        }));
    }
}

void PointCloudComputator::computeTangentPlanes()
{
    auto endIter = m_workingCloud->end();
    for(auto iter = m_workingCloud->begin(); iter != endIter; ++iter)
    {
        iter->computeTangentPlane();
    }
}


void PointCloudComputator::operator()()
{
    if(m_workingCloud->size() < 2)
    {
        return;
    }

    this->computeNeighbourhoods();
    this->computeTangentPlanes();
}
