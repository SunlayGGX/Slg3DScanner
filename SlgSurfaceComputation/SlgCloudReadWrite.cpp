#include "SlgCloudReadWrite.h"

#include "SlgBinRessource.h"
#include "PointCloudAlgorithm.h"

#include "InputCloudVertex.h"
#include "CloudVertex.h"

#include <experimental/filesystem>


std::vector<Slg3DScanner::CloudVertex> Slg3DScanner::readPointCloudfromFile(const std::string& fileName)
{
    Slg3DScanner::SlgBinRessource binRessource;
    if(binRessource.load(std::experimental::filesystem::path{ fileName }.has_extension() ? fileName : fileName + ".slgBinPos"))
    {
        std::size_t outPointCloudSize;
        std::unique_ptr<Slg3DScanner::InputCloudVertex[]> inputedCloudVertexURef{ binRessource.releaseAs<Slg3DScanner::InputCloudVertex>(outPointCloudSize) };

        if(outPointCloudSize != 0)
        {
            std::vector<Slg3DScanner::CloudVertex> result;

            PointCloudAlgorithm::computeInputCloudVertexToFinalCloudVertex(result, inputedCloudVertexURef.get(), outPointCloudSize);

            return std::move(result);
        }
    }
        
    return std::vector<Slg3DScanner::CloudVertex>{};
}

void Slg3DScanner::readPointCloud2fromFile(const std::string& fileName, Slg3DScanner::InternalCloudMesh& outCloudMesh)
{
    Slg3DScanner::SlgBinRessource binRessource;
    if(binRessource.load(std::experimental::filesystem::path{ fileName }.has_extension() ? fileName : fileName + ".slgBinPos2"))
    {
        std::size_t outPointCloudSize;
        DirectX::XMFLOAT3* scannerDir = binRessource.getAs<DirectX::XMFLOAT3>(outPointCloudSize, 0);

        Slg3DScanner::InputCloudVertex* inputedCloudVertex = binRessource.getAs<Slg3DScanner::InputCloudVertex>(outPointCloudSize, sizeof(DirectX::XMFLOAT3));

        if(outPointCloudSize != 0)
        {
            PointCloudAlgorithm::computeInputCloudVertexToFinalCloudVertexSimple(outCloudMesh, inputedCloudVertex, outPointCloudSize, *scannerDir);
        }
    }
}

bool Slg3DScanner::writePointCloudToFile(const std::string& fileName, char* data, std::size_t outPointCloudSizeInByte)
{
    Slg3DScanner::SlgBinRessource binRessource;
    binRessource.setData(data, outPointCloudSizeInByte);

    bool result;

    try
    {
        result = binRessource.write(std::experimental::filesystem::path{ fileName }.has_extension() ? fileName : fileName + ".slgBinPos");
    }
    catch(...)
    {
        //just remove ownership
        binRessource.releaseAs<char>(outPointCloudSizeInByte);
        throw;
    }

    //just remove ownership
    binRessource.releaseAs<char>(outPointCloudSizeInByte);

    return result;
}
