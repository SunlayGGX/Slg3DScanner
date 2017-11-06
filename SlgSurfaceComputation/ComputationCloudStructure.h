#pragma once

#include "CloudVertexComputationStructure.h"

namespace Slg3DScanner
{
    struct InputCloudVertex;
    struct CloudVertex;

    struct ComputationCloudBox
    {
    public:
        enum : unsigned int
        {
            X_MIN,
            X_MAX,
            Y_MIN,
            Y_MAX,
            Z_MIN,
            Z_MAX,

            LIMIT_COUNT
        };

    public:
        /*
        xmin, xmax, ymin, ymax, zmin, zmax
        */
        float m_boxLimits[ComputationCloudBox::LIMIT_COUNT];

        std::vector<CloudVertexComputationStructure> m_cloudVertexInside;
        
        std::vector<ComputationCloudBox*> m_connectedNeighbour;

    public:
        ComputationCloudBox(const float(&boxLimits)[ComputationCloudBox::LIMIT_COUNT]);
        ~ComputationCloudBox();


    public:
        bool isInsideBox(const DirectX::XMFLOAT3& pointToCheck) const;
        bool isNear(const DirectX::XMFLOAT3& pointToCheck, float distanceSquaredToConsiderNear) const;

        void bindNeighbour(ComputationCloudBox* neighbour);

        void optimize(std::size_t preallocatedPointCount);


    private:
        bool isNearPoint(const unsigned int xEdgeIndex, const unsigned int yEdgeIndex, const unsigned int zEdgeIndex, const DirectX::XMFLOAT3& pointToCheck, float distanceSquaredToConsiderNear) const;
    };

    class ComputationCloudStructure
    {
    public:
        static constexpr const float MIN_BOX_LENGTH = 0.0001f;

        enum
        {
            MAX_BOX_COUNT_ON_X_AXIS = 9,
            MAX_BOX_COUNT_ON_Y_AXIS = 9,
            MAX_BOX_COUNT_ON_Z_AXIS = 9
        };


    private:
        std::vector<ComputationCloudBox> m_boxArray;

        std::size_t m_xBoxCount;
        std::size_t m_yBoxCount;
        std::size_t m_zBoxCount;

        std::size_t m_vertexCount;


    public:
        ComputationCloudStructure(const InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount);
        ~ComputationCloudStructure();


    public:
        void compute();

        void transferToFinal(std::vector<CloudVertex>& outFinalPointCloudArray) const;


    private:
        void generateBoxArray(const InputCloudVertex* inInputedVertexes);
        void fillBoxArray(const InputCloudVertex* inInputedVertexes);

        void computeNeighbourhoods();
        void computeTangentPlanes();

        void internalEvaluateNeighbourhoodRecursive(CloudVertexComputationStructure& currentCloudVertex, ComputationCloudBox& boxNeighbour, std::vector<ComputationCloudBox*>& inOutAlreadyVisitedNeighborhood);

        void internalEraseAllNeighboursDoublons();

        void internalOptimizeBox();
        void internalOptimizeMemory();

        void computeConsistentPlaneOrientation();
    };
}
