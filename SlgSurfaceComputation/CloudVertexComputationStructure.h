#pragma once

namespace Slg3DScanner
{
    struct InputCloudVertex;
    struct ComputationCloudBox;
    class ComputationCloudStructure;

    class CloudVertexComputationStructure
    {
    private:
        friend ComputationCloudStructure;


    public:
        static constexpr const float DEFAULT_KNN_DISTANCE_SQUARED = 0.0001f;


    private:
        DirectX::XMFLOAT3 m_position;

        float m_distanceKnnSquared;

        ComputationCloudBox* m_containingBox;

        //neighbourhood
        std::vector<CloudVertexComputationStructure*> m_neighbours;
        mutable std::shared_ptr<std::mutex> m_mutex;

        //Plane
        DirectX::XMFLOAT3 m_tangentPlaneOrigin;
        DirectX::XMFLOAT3 m_tangentPlaneNormal;
        bool m_consistentlyOriented;


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

        void propagateOrientationOnNeighbourhood(const unsigned int recursiveDepth);


    private:
        void addNeighbour(CloudVertexComputationStructure* neighbour);
        void computeAverage3DCovarianceMatrixFromSetsPointsForNeighbourhood(float(&outResult)[9]);

        bool internalFindAndPropagateNormalOrientation(const unsigned int recursiveDepth, const DirectX::XMFLOAT3*& foundNormal);
        bool flipNormalIfNeeded(const DirectX::XMFLOAT3& baseNormal);
    };
}
