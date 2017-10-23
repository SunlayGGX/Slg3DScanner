#pragma once

namespace Slg3DScanner
{
    /*
    Return the equation of a linear least square regression.
    Assuming a * x + b, the return type is a XMFloat2 with first value is a, the second is b.
    */
    DirectX::XMFLOAT2 computeLinear2DLeastSquare(const std::vector<DirectX::XMFLOAT2>& data);

    DirectX::XMFLOAT3 computeLinear3DLeastSquare(const std::vector<DirectX::XMFLOAT3>& data);

    /*
    Compute centroid (= barycenter) of a set of points
    */
    DirectX::XMFLOAT3 computeCentroid(const std::vector<DirectX::XMFLOAT3>& data);



    /*
    compute the covariance matrix as :

    C = (origin - average) * transposed(origin - average).

    inOrigin and average are points.

    The result will be stored into outResult that is a 3x3 row based matrix :
    =>
    consider inOrigin - average = (a,b,c)

                      | a |                   | a^2 ab  ac  |
    we have :     C = | b | * (a b c) =       | ab  b^2 bc  | 
                      | c |                   | ca  cb  c^2 | 

    and outResult will be formed like this :
    (a^2 ab ac ab b^2 bc ca cb c^2)
    */
    void compute3DCovarianceMatrix(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& average, float(&outResult)[9]);

    /*
    Optimized minimal version
    compute the covariance matrix as :

    C = (origin - average) * transposed(origin - average).

    inOrigin and average are points.

    The result will be stored into outResult that is a 3x3 row based matrix :
    =>
    consider inOrigin - average = (a,b,c)

                      | a |                   | a^2 ab  ac  |
    we have :     C = | b | * (a b c) =       | ab  b^2 bc  |
                      | c |                   | ca  cb  c^2 |

    and outResult will be formed like this :
    (a^2 ab ac b^2 bc ca c^2) => coeff 3, 6 and 7 can be deduced by 1, 2, 5 respectively
    */
    void compute3DCovarianceMatrixMinimal(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& average, float(&outResult)[6]);

    /*
    Correspond to :
    C = 1 / k * sum((neighbours - neighboursCentroid) * transposed(neighbours - neighboursCentroid)) from neighbour(0) to neighbour(k-1) included.
    */
    void computeAverage3DCovarianceMatrixFromSetsPoints(const std::vector<DirectX::XMFLOAT3>& neighbours, const DirectX::XMFLOAT3& neighboursCentroid, float(&outResult)[9]);

    void computeEigenValueAndEigenVectorsFromCovarianceMatrix(const float(&inCovarianceMatrix)[9], DirectX::XMFLOAT3& outEigenValues, float(&outEigenVectors)[9]);

    void computeEigenValueAndEigenVectorsFromCovarianceMatrixAndOutputNormal(const float(&inCovarianceMatrix)[9], DirectX::XMFLOAT3& outNormal);
}
