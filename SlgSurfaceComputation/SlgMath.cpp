#include "SlgSurfaceComputationPCH.h"
#include "SlgMath.h"

#include <cmath>
#include <Eigenvalues>

DirectX::XMFLOAT2 Slg3DScanner::computeLinear2DLeastSquare(const std::vector<DirectX::XMFLOAT2>& data)
{
    float xAverage = 0.f;
    float yAverage = 0.f;
    
    auto endIter = data.end();
    for(auto iter = data.begin(); iter != endIter; ++iter)
    {
        xAverage += iter->x;
        yAverage += iter->y;
    }

    float count = static_cast<float>(data.size());

    xAverage /= count;
    yAverage /= count;

    float numerator = 0.f;
    float denominator = 0.f;

    auto dataEndIter = data.end();
    for(auto dataIter = data.begin(); dataIter != dataEndIter; ++dataIter)
    {
        float xMinusXAverage = dataIter->x - xAverage;

        numerator += xMinusXAverage * (dataIter->y - yAverage);
        denominator += xMinusXAverage * xMinusXAverage;
    }

    float linearCoeff = numerator / denominator;

    return { linearCoeff, yAverage - (xAverage * linearCoeff) };
}

DirectX::XMFLOAT3 Slg3DScanner::computeLinear3DLeastSquare(const std::vector<DirectX::XMFLOAT3>& data)
{
    /*
    for result as z minimization z = Ax + By + C
    | a b c |   | A |   | r1 |
    | b e f | * | B | = | r2 |
    | c f i |   | C |   | r3 |
    */

    enum
    {
        c,
        a,
        r1,
        b,
        f,
        e,
        r2,
        r3,
    };

    //c, a, r1, b, f, e, r2, r3
    float coefficient[8] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

    //i = sum(1) for i = 1 to m => the count. No need to compute it...
    float i = static_cast<float>(data.size());

    auto endIter = data.end();
    for(auto iter = data.begin(); iter != endIter; ++iter)
    {
        const DirectX::XMFLOAT3& current = *iter;

        coefficient[c] += current.x;
        coefficient[a] += current.x * current.x;
        coefficient[r1] += current.x * current.z;
        coefficient[b] += current.x * current.y;
        coefficient[f] += current.y;
        coefficient[e] += current.y * current.y;
        coefficient[r2] += current.y * current.z;
        coefficient[r3] += current.z;
    }

    float cSquared = coefficient[c] * coefficient[c];
    float bSquared = coefficient[b] * coefficient[b];
    float fSquared = coefficient[f] * coefficient[f];

    float af = coefficient[a] * coefficient[f];
    float cb = coefficient[c] * coefficient[b];
    float cf = coefficient[c] * coefficient[f];
    float ce = coefficient[c] * coefficient[e];
    float bf = coefficient[b] * coefficient[f];
    float bi = coefficient[b] * i;
    float ei = coefficient[e] * i;

    //sarrus : aei + 2cbf - ec^2 - af^2 - ib^2
    float determinant =
        coefficient[a] * ei +
        cb * coefficient[f] * 2.f -
        coefficient[e] * cSquared -
        coefficient[a] * fSquared -
        i * bSquared;

    float A = ((ei - fSquared) * coefficient[r1] + (cf - bi) * coefficient[r2] + (bf - ce) * coefficient[r3]) / determinant;

    float B = ((cf - bi) * coefficient[r1] + (coefficient[a] * i - cSquared) * coefficient[r2] + (cb - af) * coefficient[r3]) / determinant;

    float C = ((bf - ce) * coefficient[r1] + (cb - af) * coefficient[r2] + (coefficient[a] * coefficient[e] - bSquared) * coefficient[r3]) / determinant;

    return{ A, B, C };
}

DirectX::XMFLOAT3 Slg3DScanner::computeCentroid(const std::vector<DirectX::XMFLOAT3>& data)
{
    DirectX::XMFLOAT3 result{ 0.f, 0.f, 0.f };

    auto endIter = data.end();
    for(auto iter = data.begin(); iter != endIter; ++iter)
    {
        auto& current = *iter;
        result.x += current.x;
        result.y += current.y;
        result.z += current.z;
    }

    float sizeF = static_cast<float>(data.size());

    result.x /= sizeF;
    result.y /= sizeF;
    result.z /= sizeF;

    return result;
}

void Slg3DScanner::compute3DCovarianceMatrix(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& average, float(&outResult)[9])
{
    DirectX::XMFLOAT3 delta{ origin.x - average.x, origin.y - average.y, origin.z - average.z };

    outResult[0] = delta.x * delta.x;
    outResult[1] = delta.x * delta.y;
    outResult[2] = delta.x * delta.z;
    outResult[3] = outResult[1];
    outResult[4] = delta.y * delta.y;
    outResult[5] = delta.y * delta.z;
    outResult[6] = outResult[2];
    outResult[7] = outResult[5];
    outResult[8] = delta.z * delta.z;
}

void Slg3DScanner::compute3DCovarianceMatrixMinimal(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& average, float(&outResult)[6])
{
    DirectX::XMFLOAT3 delta{ origin.x - average.x, origin.y - average.y, origin.z - average.z };

    outResult[0] = delta.x * delta.x;
    outResult[1] = delta.x * delta.y;
    outResult[2] = delta.x * delta.z;
    outResult[3] = delta.y * delta.y;
    outResult[4] = delta.y * delta.z;
    outResult[5] = delta.z * delta.z;
}

void Slg3DScanner::computeAverage3DCovarianceMatrixFromSetsPoints(const std::vector<DirectX::XMFLOAT3>& neighbours, const DirectX::XMFLOAT3& neighboursCentroid, float(&outResult)[9])
{
    memset(outResult, 0, sizeof(outResult));

    float tempCovariance[6];

    auto endIter = neighbours.end();
    for(auto iter = neighbours.begin(); iter != endIter; ++iter)
    {
        Slg3DScanner::compute3DCovarianceMatrixMinimal(*iter, neighboursCentroid, tempCovariance);

        outResult[0] += tempCovariance[0];
        outResult[1] += tempCovariance[1];
        outResult[2] += tempCovariance[2];
        outResult[4] += tempCovariance[3];
        outResult[5] += tempCovariance[4];
        outResult[8] += tempCovariance[5];
    }

    float sizeF = static_cast<float>(neighbours.size());

    outResult[0] /= sizeF;
    outResult[1] /= sizeF;
    outResult[2] /= sizeF;
    outResult[4] /= sizeF;
    outResult[5] /= sizeF;
    outResult[8] /= sizeF;

    //covariance matrix are always symetric
    outResult[3] = outResult[1];
    outResult[6] = outResult[2];
    outResult[7] = outResult[5];
}

void Slg3DScanner::computeEigenValueAndEigenVectorsFromCovarianceMatrix(const float(&inCovarianceMatrix)[9], DirectX::XMFLOAT3& outEigenValues, float(&outEigenVectors)[9])
{
    using MatrixType = Eigen::Matrix<float, 3, 3>;
    
    Eigen::EigenSolver<MatrixType> solver{ MatrixType{ inCovarianceMatrix } };
    auto eigenValues = solver.eigenvalues();
    auto eigenVectors = solver.eigenvectors();

    outEigenValues.x = eigenValues.x().real();
    outEigenValues.y = eigenValues.y().real();
    outEigenValues.z = eigenValues.z().real();

    for(auto iter = 0; iter < 9; ++iter)
    {
        outEigenVectors[iter] = eigenVectors.data()[iter].real();
    }
}

void Slg3DScanner::computeEigenValueAndEigenVectorsFromCovarianceMatrixAndOutputNormal(const float(&inCovarianceMatrix)[9], DirectX::XMFLOAT3& outNormal)
{
    using MatrixType = Eigen::Matrix<float, 3, 3>;

    Eigen::EigenSolver<MatrixType> solver{ MatrixType{ inCovarianceMatrix } };
    auto& eigenValues = solver.eigenvalues();
    auto& eigenVectors = solver.eigenvectors();
    
    //don't know if it is a good idea but for now, I will keep only real part, not imaginary parts... I'll see if it is a good idea when testing...
    auto minElement = std::min_element(
        eigenValues.data(), 
        eigenValues.data() + 3, 
        [](const std::complex<float>& a, const std::complex<float>& b) 
    {
        return a.real() < b.real();
    });

    auto normal = &eigenVectors(minElement - eigenValues.data());

    outNormal.x = normal->real();
    outNormal.y = (normal + 3)->real();
    outNormal.z = (normal + 6)->real();

    float norme = std::sqrtf(outNormal.x * outNormal.x + outNormal.y * outNormal.y + outNormal.z * outNormal.z);

    constexpr const float epsilon = 1e-10f;
    if(std::abs(norme) > epsilon)
    {
        outNormal.x /= norme;
        outNormal.y /= norme;
        outNormal.z /= norme;
    }
    else
    {
        outNormal = { 0.f, 0.f, 0.f };
    }
}

float Slg3DScanner::scalar(const DirectX::XMFLOAT3& vect1, const DirectX::XMFLOAT3& vect2)
{
    return
        vect1.x * vect2.x +
        vect1.y * vect2.y +
        vect1.z * vect2.z;
}

float Slg3DScanner::scalar(const DirectX::XMFLOAT2& vect1, const DirectX::XMFLOAT2& vect2)
{
    return
        vect1.x * vect2.x +
        vect1.y * vect2.y;
}

DirectX::XMFLOAT3 Slg3DScanner::cross(const DirectX::XMFLOAT3& vect1, const DirectX::XMFLOAT3& vect2)
{
    return
    {
        vect1.y * vect2.z - vect1.z * vect2.y,
        vect1.z * vect2.x - vect1.x * vect2.z,
        vect1.x * vect2.y - vect1.y * vect2.x
    };
}

float Slg3DScanner::lengthSquared(const DirectX::XMFLOAT3& vect)
{
    return Slg3DScanner::scalar(vect, vect);
}

float Slg3DScanner::lengthSquared(const DirectX::XMFLOAT2& vect)
{
    return Slg3DScanner::scalar(vect, vect);
}

DirectX::XMFLOAT3 Slg3DScanner::getNormalized(const DirectX::XMFLOAT3& vect)
{
    float norm = std::sqrtf(lengthSquared(vect));
    return { vect.x / norm, vect.y / norm, vect.z / norm };
}

void Slg3DScanner::normalize(DirectX::XMFLOAT2& inOutVect)
{
    float norm = std::sqrtf(lengthSquared(inOutVect));
    inOutVect.x /= norm;
    inOutVect.y /= norm;
}
