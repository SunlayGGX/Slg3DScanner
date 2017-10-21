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
                static std::string findFolderPathWithOffsetFromCurrentPath(const std::string& folderName, int offset)
                {
                    auto specified = std::experimental::filesystem::current_path();
                    while(offset != 0 && specified.has_parent_path())
                    {
                        specified = specified.parent_path();
                        --offset;
                    }

                    return FolderFinderPImpl::findRecursiveFolderPathFromCurrentPath(
                        folderName,
                        std::experimental::filesystem::directory_iterator{ specified }
                    );
                }

                static std::string findFolderPathFromCurrentPath(const std::string& folderName)
                {
                    return
                        FolderFinderPImpl::findRecursiveFolderPathFromCurrentPath(
                            folderName,
                            std::experimental::filesystem::directory_iterator{ std::experimental::filesystem::current_path() }
                    );
                }

                static std::string findFolderPathFromSpecifiedPath(const std::string& folderName, const std::string& path)
                {
                    std::experimental::filesystem::path specifiedPath{ path };
                    if(std::experimental::filesystem::exists(specifiedPath))
                    {
                        return
                            FolderFinderPImpl::findRecursiveFolderPathFromCurrentPath(
                                folderName,
                                std::experimental::filesystem::directory_iterator{ specifiedPath }
                        );
                    }
                    else
                    {
                        throw std::exception{ ("invalid begin path : " + path + " doesn't exists").c_str() };
                    }
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


                static std::string findStructurePathFromCurrentPath(const std::string& structureName)
                {
                    return
                        FolderFinderPImpl::findStructurePathFromCurrentPath(
                            structureName,
                            std::experimental::filesystem::directory_iterator{ std::experimental::filesystem::current_path() }
                    );
                }

                static std::string findStructurePathFromSpecifiedPath(const std::string& structureName, const std::string& path)
                {
                    std::experimental::filesystem::path specifiedPath{ path };
                    if(std::experimental::filesystem::exists(specifiedPath))
                    {
                        return
                            FolderFinderPImpl::findStructurePathFromCurrentPath(
                                structureName,
                                std::experimental::filesystem::directory_iterator{ specifiedPath }
                        );
                    }
                    else
                    {
                        throw std::exception{ ("invalid begin path : " + path + " doesn't exists").c_str() };
                    }
                }

                static std::string findStructurePathFromCurrentPath(const std::string& structureName, std::experimental::filesystem::directory_iterator folderIter)
                {
                    auto endFolderIter = std::experimental::filesystem::directory_iterator{};
                    for(; folderIter != endFolderIter; ++folderIter)
                    {
                        auto currentIterFolderPath = folderIter->path();
                        if(currentIterFolderPath.filename().string() == structureName || currentIterFolderPath.stem().string() == structureName)
                        {
                            return currentIterFolderPath.string();
                        }
                        else if(std::experimental::filesystem::is_directory(folderIter->status()))
                        {
                            std::string found = std::move(FolderFinderPImpl::findStructurePathFromCurrentPath(
                                structureName,
                                std::experimental::filesystem::directory_iterator{ currentIterFolderPath }
                            ));

                            if(!found.empty())
                            {
                                return found;
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

        std::string FolderFinder::findFolderPathWithSpecifiedOffsetFromCurrentPath(const std::string& folderName, int offset)
        {
            std::string found = PImpl::FolderFinderPImpl::findFolderPathWithOffsetFromCurrentPath(folderName, offset);

            if(!found.empty())
            {
                return found;
            }
            else
            {
                throw std::exception{ (folderName + " not found or isn't a folder!").c_str() };
            }
        }

        std::string FolderFinder::findFolderPathFromSpecifiedPath(const std::string& folderName, const std::string& path)
        {
            std::string found = PImpl::FolderFinderPImpl::findFolderPathFromSpecifiedPath(folderName, path);

            if(!found.empty())
            {
                return found;
            }
            else
            {
                throw std::exception{ (folderName + " not found or isn't a folder!").c_str() };
            }
        }

        std::string FolderFinder::findStructurePathFromCurrentPath(const std::string& StructureName)
        {
            std::string found = PImpl::FolderFinderPImpl::findStructurePathFromCurrentPath(StructureName);

            if(!found.empty())
            {
                return found;
            }
            else
            {
                throw std::exception{ (StructureName + " not found!").c_str() };
            }
        }

        std::string FolderFinder::findStructurePathFromSpecifiedPath(const std::string& StructureName, const std::string& path)
        {
            std::string found = PImpl::FolderFinderPImpl::findStructurePathFromSpecifiedPath(StructureName, path);

            if(!found.empty())
            {
                return found;
            }
            else
            {
                throw std::exception{ (StructureName + " not found!").c_str() };
            }
        }
    }
}
