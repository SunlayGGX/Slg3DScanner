#pragma once


namespace Slg3DScanner
{
    struct CloudVertex;

    /*
    Read a point cloud from a SlgBinPos file. No need to specify the extension => if the name is "toto.slgBinPos", enter "toto". But if you want a custom extension, specify it.
    If the operation was successful, it will return a non null unique ptr and outPointCloudSize will be set to the point cloud count.
    */
    extern std::unique_ptr<CloudVertex[]> readPointCloudfromFile(const std::string& fileName, std::size_t& outPointCloudSize);
    
    /*
    Write a point cloud from a SlgBinPos file. No need to specify the extension => if the name is "toto.slgBinPos", enter "toto". But if you want a custom extension, specify it.
    If the operation was successful, it will return a non null unique ptr and outPointCloudSize will be set to the point cloud count.
    */
    extern bool writePointCloudToFile(const std::string& fileName, char* data, std::size_t outPointCloudSizeInByte);
}
