#include "SlgCloudReadWrite.h"

#include "SlgBinRessource.h"
#include "CloudVertex.h"

#include <experimental/filesystem>

std::unique_ptr<Slg3DScanner::CloudVertex[]> Slg3DScanner::readPointCloudfromFile(const std::string& fileName, std::size_t& outPointCloudSize)
{
    Slg3DScanner::SlgBinRessource binRessource;
    if(binRessource.load(std::experimental::filesystem::path{ fileName }.has_extension() ? fileName : fileName + ".slgBinPos"))
    {
        return std::unique_ptr<Slg3DScanner::CloudVertex[]>(binRessource.releaseAs<Slg3DScanner::CloudVertex>(outPointCloudSize));
    }
    else
    {
        outPointCloudSize = 0;
        return std::unique_ptr<Slg3DScanner::CloudVertex[]>{};
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
