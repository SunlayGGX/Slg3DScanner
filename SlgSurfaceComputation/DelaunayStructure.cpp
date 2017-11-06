#include "DelaunayStructure.h"

using namespace Slg3DScanner;


float DelaunayPoint2d::lengthSquared() const
{
    return x * x + y * y;
}

void DelaunayPoint2d::normalize()
{
    float length = sqrtf(this->lengthSquared());
    if(length != 0.f)
    {
        x /= length;
        y /= length;
    }
}

float DelaunayPoint2d::scalar(const DelaunayPoint2d& vect1, const DelaunayPoint2d& vect2)
{
    return vect1.x * vect2.x + vect1.y * vect2.y;
}

void DelaunayPoint2d::operator+=(const DelaunayPoint2d& a)
{
    this->x += a.x;
    this->y += a.y;
}

DelaunayPoint2d DelaunayPoint2d::operator+(DelaunayPoint2d a) const
{
    a += *this;
    return a;
}

void DelaunayPoint2d::operator*=(float scalar)
{
    this->x *= scalar;
    this->y *= scalar;
}

DelaunayPoint2d DelaunayPoint2d::operator*(float scalar) const
{
    DelaunayPoint2d tmp{ *this };
    tmp *= scalar;
    return tmp;
}

void DelaunayPoint2d::operator/=(float scalar)
{
    this->x /= scalar;
    this->y /= scalar;
}

DelaunayPoint2d DelaunayPoint2d::operator/(float scalar) const
{
    DelaunayPoint2d tmp{ *this };
    tmp /= scalar;
    return tmp;
}

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

float DelaunayTriangle::computeArea(const std::vector<DelaunayPoint2d>& pointList) const
{
    return DelaunayTriangle::computeArea(pointList[index0], pointList[index1], pointList[index2]);
}

float DelaunayTriangle::computeArea(const DelaunayPoint2d& A, const DelaunayPoint2d& B, const DelaunayPoint2d& C)
{
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

bool DelaunayTriangle::isCounterClockwise(const std::vector<DelaunayPoint2d>& pointList, float& outArea) const
{
    outArea = this->computeArea(pointList);
    return outArea > 0.f;
}
