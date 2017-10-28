#pragma once

namespace Slg3DScanner
{
    struct InputCloudVertex;

    class CloudVertexComputationStructure
    {
    public:
        static constexpr const float DEFAULT_KNN_DISTANCE_SQUARED = 0.00000025f;


    public:
        DirectX::XMFLOAT3 m_position;

        float m_distanceKnnSquared;


    private:
        //neighbourhood
        std::vector<CloudVertexComputationStructure*> m_neighbours;
        mutable std::shared_ptr<std::mutex> m_mutex;

        //Plane
        DirectX::XMFLOAT3 m_tangentPlaneOrigin;
        DirectX::XMFLOAT3 m_tangentPlaneNormal;


    public:
        CloudVertexComputationStructure(const InputCloudVertex& inputPoint);
        CloudVertexComputationStructure(CloudVertexComputationStructure&);
        CloudVertexComputationStructure(CloudVertexComputationStructure&&);
        ~CloudVertexComputationStructure();

        CloudVertexComputationStructure& operator=(const CloudVertexComputationStructure&);
        CloudVertexComputationStructure& operator=(CloudVertexComputationStructure&& other);


    public:
        void evaluateNeighbourhood(CloudVertexComputationStructure& newNeighbour);
        void computeTangentPlane();

        void swap(CloudVertexComputationStructure& other);


    private:
        void addNeighbour(CloudVertexComputationStructure* neighbour);
        void computeAverage3DCovarianceMatrixFromSetsPointsForNeighbourhood(float(&outResult)[9]);
    };
}
