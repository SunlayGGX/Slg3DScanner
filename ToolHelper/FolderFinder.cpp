#include "ToolHelperPCH.h"
#include "FolderFinder.h"

#include <filesystem>

namespace Slg3DScanner
{
    namespace SlgTools
    {
        namespace PImpl
        {
            struct FolderFinderPImpl
            {
            private:
                FolderFinderPImpl() = delete;
                ~FolderFinderPImpl() = delete;
                FolderFinderPImpl(FolderFinderPImpl&) = delete;
                FolderFinderPImpl& operator=(FolderFinderPImpl&) = delete;

            public:
                static std::string findFolderPathFromCurrentPath(const std::string& folderName)
                {
                    return
                        FolderFinderPImpl::findRecursiveFolderPathFromCurrentPath(
                            folderName,
                            std::experimental::filesystem::directory_iterator{ std::experimental::filesystem::current_path() }
                    );
                }

                static std::string findRecursiveFolderPathFromCurrentPath(const std::string& folderName, std::experimental::filesystem::directory_iterator folderIter)
                {
                    auto endFolderIter = std::experimental::filesystem::directory_iterator{};
                    for(; folderIter != endFolderIter; ++folderIter)
                    {
                        if(std::experimental::filesystem::is_directory(folderIter->status()))
                        {
                            auto currentIterFolderPath = folderIter->path();
                            if(currentIterFolderPath.filename().string() == folderName)
                            {
                                return currentIterFolderPath.string();
                            }
                            else
                            {
                                std::string found = std::move(FolderFinderPImpl::findRecursiveFolderPathFromCurrentPath(
                                    folderName, 
                                    std::experimental::filesystem::directory_iterator{ currentIterFolderPath }
                                ));

                                if(!found.empty())
                                {
                                    return found;
                                }
                            }
                        }
                    }

                    return std::string{};
                }
            };
        }

        std::string FolderFinder::findFolderPathFromCurrentPath(const std::string& folderName)
        {
            std::string found = PImpl::FolderFinderPImpl::findFolderPathFromCurrentPath(folderName);

            if(!found.empty())
            {
                return found;
            }
            else
            {
                throw std::exception{ (folderName + " not found or isn't a folder!").c_str() };
            }
        }
    }
}
