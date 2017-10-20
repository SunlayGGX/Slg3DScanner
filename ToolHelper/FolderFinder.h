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
        };
    }
}
