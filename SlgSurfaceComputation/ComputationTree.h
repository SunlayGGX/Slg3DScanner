#pragma once

namespace Slg3DScanner
{
    struct InputCloudVertex;

    template<class CloudVertexType>
    struct ComputationTreeCloudBox
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
        float m_boxLimits[ComputationTreeCloudBox::LIMIT_COUNT];

        std::vector<CloudVertexType> m_cloudVertexInside;

        std::vector<ComputationTreeCloudBox*> m_connectedNeighbour;

    public:
        ComputationTreeCloudBox(const float(&boxLimits)[ComputationTreeCloudBox::LIMIT_COUNT]);
        ~ComputationTreeCloudBox();


    public:
        bool isInsideBox(const DirectX::XMFLOAT3& pointToCheck) const;
        bool isNear(const DirectX::XMFLOAT3& pointToCheck, float distanceSquaredToConsiderNear) const;

        void bindNeighbour(ComputationTreeCloudBox* neighbour);

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
        //std::vector<ComputationCloudBox> m_boxArray;

        std::size_t m_xBoxCount;
        std::size_t m_yBoxCount;
        std::size_t m_zBoxCount;

        std::size_t m_vertexCount;


    public:
        ComputationCloudStructure(const InputCloudVertex* inInputedVertexes, std::size_t inputedVertexCount);
    };

    class ComputationTree
    {
    public:
        ComputationTree();
        ~ComputationTree();

    private:
        void generateBoxArray(const InputCloudVertex* inInputedVertexes);
        void fillBoxArray(const InputCloudVertex* inInputedVertexes);
    };
}
