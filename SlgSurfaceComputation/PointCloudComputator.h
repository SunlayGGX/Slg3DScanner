#pragma once


namespace Slg3DScanner
{
    class CloudVertexComputationStructure;

    class PointCloudComputator
    {
    private:
        std::vector<CloudVertexComputationStructure>* m_workingCloud;

    public:
        PointCloudComputator(std::vector<CloudVertexComputationStructure>& workingCloud);
        ~PointCloudComputator();

        void setWorkingPointCloud(std::vector<CloudVertexComputationStructure>& workingCloud);

    private:
        void computeNeighbourhoods();
        void computeTangentPlanes();

    public:
        void operator()();
    };
}
