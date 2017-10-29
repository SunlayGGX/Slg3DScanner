#pragma once

namespace Slg3DScanner
{
    struct InputCloudVertex;
    struct ComputationCloudBox;

    class CloudVertexComputationStructure
    {
    public:
        static constexpr const float DEFAULT_KNN_DISTANCE_SQUARED = 0.0004f;


    public:
        DirectX::XMFLOAT3 m_position;

        float m_distanceKnnSquared;


    private:
        ComputationCloudBox* m_containingBox;

        //neighbourhood
        std::vector<CloudVertexComputationStructure*> m_neighbours;
        mutable std::shared_ptr<std::mutex> m_mutex;

        //Plane
        DirectX::XMFLOAT3 m_tangentPlaneOrigin;
        DirectX::XMFLOAT3 m_tangentPlaneNormal;


    public:
        CloudVertexComputationStructure(const InputCloudVertex& inputPoint, ComputationCloudBox& box);
        CloudVertexComputationStructure(CloudVertexComputationStructure&);
        CloudVertexComputationStructure(CloudVertexComputationStructure&&);
        ~CloudVertexComputationStructure();

        CloudVertexComputationStructure& operator=(const CloudVertexComputationStructure&);
        CloudVertexComputationStructure& operator=(CloudVertexComputationStructure&& other);


    public:
        ComputationCloudBox* getContainingBox() const;

        void evaluateNeighbourhood(CloudVertexComputationStructure& newNeighbour);
        void computeTangentPlane();

        void swap(CloudVertexComputationStructure& other);

        void optimizeAllocSpeed(std::size_t newNeighbourCapacity);
        void optimizeAllocMemory();

        void eraseNeighbourNoDoublon();


    private:
        void addNeighbour(CloudVertexComputationStructure* neighbour);
        void computeAverage3DCovarianceMatrixFromSetsPointsForNeighbourhood(float(&outResult)[9]);
    };
}
