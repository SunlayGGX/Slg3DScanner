#pragma once

namespace Slg3DScanner
{
    namespace SlgTools
    {
        class FolderFinder
        {
        private:
            FolderFinder() = delete;
            ~FolderFinder() = delete;
            FolderFinder(FolderFinder&) = delete;
            FolderFinder& operator=(FolderFinder&) = delete;

        public:
            static std::string findFolderPathFromCurrentPath(const std::string& folderName);
            static std::string findFolderPathWithSpecifiedOffsetFromCurrentPath(const std::string& folderName, int offset);
            static std::string findFolderPathFromSpecifiedPath(const std::string& folderName, const std::string& path);
            static std::string findStructurePathFromCurrentPath(const std::string& StructureName);
            static std::string findStructurePathFromSpecifiedPath(const std::string& StructureName, const std::string& path);
        };
    }
}
