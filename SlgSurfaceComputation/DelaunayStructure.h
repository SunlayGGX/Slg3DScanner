#pragma once

namespace Slg3DScanner
{
    struct DelaunayPoint2d
    {
    public:
        float x;
        float y;

    public:
        float lengthSquared() const;
        void normalize();

        void operator+=(const DelaunayPoint2d& a);
        DelaunayPoint2d operator+(DelaunayPoint2d a) const;

        void operator*=(float scalar);
        DelaunayPoint2d operator*(float scalar) const;
        void operator/=(float scalar);
        DelaunayPoint2d operator/(float scalar) const;

    public:
        static float scalar(const DelaunayPoint2d& vect1, const DelaunayPoint2d& vect2);
    };

    struct DelaunayEdge
    {
        std::size_t index0;
        std::size_t index1;
    };

    struct DelaunayTriangle
    {
    public:
        std::size_t index0 = std::numeric_limits<std::size_t>::max();
        std::size_t index1 = std::numeric_limits<std::size_t>::max();
        std::size_t index2 = std::numeric_limits<std::size_t>::max();

    public:
        DelaunayTriangle(std::size_t id0);
        DelaunayTriangle(std::size_t id0, std::size_t id1);
        DelaunayTriangle(std::size_t id0, std::size_t id1, std::size_t id2);

    public:
        float computeArea(const std::vector<DelaunayPoint2d>& pointList) const;
        bool isCounterClockwise(const std::vector<DelaunayPoint2d>& pointList, float& outArea) const;

    public:
        static float computeArea(const DelaunayPoint2d& A, const DelaunayPoint2d& B, const DelaunayPoint2d& C);
    };
}
