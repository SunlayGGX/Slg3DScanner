#include "Slg3DScannerPackagerPCH.h"
#include "AsynchronousCopier.h"

#include "IgnoreParser.h"

#include "Slg3DScannerPackagerLogUtils.h"

#include <thread>

using namespace Slg3DScanner::Tools;

std::mutex AsynchronousCopier::s_creationMutex;

AsynchronousCopier::AsynchronousCopier() = default;

AsynchronousCopier::~AsynchronousCopier() = default;


void AsynchronousCopier::bindSourceToDestinationPath(std::experimental::filesystem::path sourcePath, std::experimental::filesystem::path destinationPath, bool fileUniCopy)
{
    if(std::experimental::filesystem::exists(sourcePath))
    {
        std::lock_guard<std::mutex> autoLocker{ m_mutex };

        if(fileUniCopy)
        {
            m_sourceAndDestinationFileBindPath.emplace_back(std::move(sourcePath), std::move(destinationPath));
        }
        else
        {
            m_sourceAndDestinationFolderBindPath.emplace_back(std::move(sourcePath), std::move(destinationPath));
        }
    }
    else
    {
        throw std::exception{ ("The source to copy at " + sourcePath.string() + " doesn't exists").c_str() };
    }
}

void AsynchronousCopier::internalCreateDestinationDirIfDoesntExists(const std::experimental::filesystem::path& destinationPath, const bool isFile)
{
    std::lock_guard<std::mutex> autoLocker{ s_creationMutex };
    if(!std::experimental::filesystem::is_directory(destinationPath))
    {
        if(!std::experimental::filesystem::exists(destinationPath))
        {
            std::vector<std::experimental::filesystem::path> folderToCreate;
            folderToCreate.reserve(8);

            folderToCreate.emplace_back(destinationPath);

            while(folderToCreate.back().has_parent_path())
            {
                auto pathToEval = folderToCreate.back().parent_path();
                if(!std::experimental::filesystem::exists(pathToEval))
                {
                    folderToCreate.emplace_back(std::move(pathToEval));
                }
                else
                {
                    break;
                }
            }

            if(isFile && folderToCreate.size() < 2)
            {
                return;
            }

            try
            {
                auto firstIter = folderToCreate.rbegin();
                auto endIter = isFile ? (folderToCreate.rend() - 1) : folderToCreate.rend();

                for(auto currentIter = firstIter; currentIter != endIter; ++currentIter)
                {
                    LOG(std::endl << *currentIter << " doesn't exists. Creating it now as a folder...");
                    if(std::experimental::filesystem::create_directory(*currentIter))
                    {
                        LOG("Success : " << *currentIter << " folder was created");
                    }
                    else
                    {
                        if(currentIter != firstIter)
                        {
                            std::experimental::filesystem::remove(*firstIter);
                        }
                        throw std::exception{ ("Failed to create directory " + currentIter->string() + ". Cancelling").c_str() };
                    }
                }
            }
            catch(const std::experimental::filesystem::filesystem_error& exception)
            {
                LOG("Failed to create directory " << destinationPath.string() << "due to : " << exception.what());
                throw;
            }
        }
        else
        {
            throw std::exception{ ("Destination for copy path at " + destinationPath.string() + " is not a directory").c_str() };
        }
    }
}

void AsynchronousCopier::copyRelevantDirectoryRecursive(
    const std::experimental::filesystem::path& destinationPath, 
    std::experimental::filesystem::directory_iterator directoryIterator, 
    const std::vector<std::experimental::filesystem::path>& ignore
)
{
    std::experimental::filesystem::directory_iterator endDirIter;
    for(; directoryIterator != endDirIter; ++directoryIterator)
    {
        std::experimental::filesystem::path name = directoryIterator->path().filename();
        std::experimental::filesystem::path extension = directoryIterator->path().extension();
        std::experimental::filesystem::path stem = directoryIterator->path().stem();

        if(std::find_if(ignore.begin(), ignore.end(),
            [&extension, &name, &stem](const std::experimental::filesystem::path& ignoreEval) {
            return extension == ignoreEval || name == ignoreEval || stem == ignoreEval;
        }) == ignore.end())
        {
            auto mirrorredPath = destinationPath / name;

            auto status = directoryIterator->status();
            if(std::experimental::filesystem::is_directory(status))
            {
                if(std::experimental::filesystem::exists(mirrorredPath))
                {
                    if(std::experimental::filesystem::is_directory(mirrorredPath))
                    {
                        LOG("SUCCESS : " << mirrorredPath << " folder created successfully");
                        AsynchronousCopier::copyRelevantDirectoryRecursive(
                            mirrorredPath,
                            std::experimental::filesystem::directory_iterator{ directoryIterator->path() },
                            ignore
                        );
                    }
                    else
                    {
                        throw std::exception{ (mirrorredPath.string() + " already exists but isn't a folder! Aborting").c_str() };
                    }
                }
                else if(std::experimental::filesystem::create_directory(mirrorredPath))
                {
                    LOG("SUCCESS : " << mirrorredPath << " folder created successfully");
                    AsynchronousCopier::copyRelevantDirectoryRecursive(
                        mirrorredPath,
                        std::experimental::filesystem::directory_iterator{ directoryIterator->path() },
                        ignore
                    );
                }
                else
                {
                    LOG("FAILED : " << mirrorredPath << " folder wasn't created");
                }
            }
            else
            {
                if(std::experimental::filesystem::copy_file(directoryIterator->path(), mirrorredPath, std::experimental::filesystem::copy_options::update_existing))
                {
                    LOG("SUCCESS : " << mirrorredPath << " copied from " << directoryIterator->path());
                }
                else
                {
                    LOG("FAILED : " << mirrorredPath << " wasn't copied (original at " << directoryIterator->path() << ")");
                }
            }
        }
    }
}

void AsynchronousCopier::copyFile(const std::experimental::filesystem::path& sourcePath, const std::experimental::filesystem::path& destinationPath, const std::vector<std::experimental::filesystem::path>& ignore)
{
    std::experimental::filesystem::copy(sourcePath, destinationPath, std::experimental::filesystem::copy_options::update_existing);
}

void AsynchronousCopier::start(const IgnoreParser& ignoreParser, AsynchronousCopier::WaitForCompletion)
{
    std::vector<std::thread> threadP;

    {
        std::lock_guard<std::mutex> autoLocker{ m_mutex };

        threadP.reserve(m_sourceAndDestinationFileBindPath.empty() ? m_sourceAndDestinationFolderBindPath.size() : (m_sourceAndDestinationFolderBindPath.size() + 1));

        BindingPathArray arrayBind;
        std::swap(arrayBind, m_sourceAndDestinationFileBindPath);

        threadP.emplace_back(
            [
                fileCopyBindArray = std::move(arrayBind),
                toIgnoreData = ignoreParser
            ]()
        {
            auto endIter = fileCopyBindArray.end();
            for(auto iter = fileCopyBindArray.begin(); iter != endIter; ++iter)
            {
                AsynchronousCopier::internalCreateDestinationDirIfDoesntExists(iter->second, false /*The destination is a directory*/);
                AsynchronousCopier::copyFile(iter->first, iter->second, toIgnoreData.getAllExtensionToIgnore());
            }
        });

        auto endIter = m_sourceAndDestinationFolderBindPath.end();
        for(auto iter = m_sourceAndDestinationFolderBindPath.begin(); iter != endIter; ++iter)
        {
            threadP.emplace_back(
                [
                    source = std::move(iter->first),
                    destination = std::move(iter->second),
                    toIgnoreData = ignoreParser
                ]()
            {
                AsynchronousCopier::internalCreateDestinationDirIfDoesntExists(destination, false);

                AsynchronousCopier::copyRelevantDirectoryRecursive(
                    destination,
                    std::experimental::filesystem::directory_iterator{ source },
                    toIgnoreData.getAllExtensionToIgnore()
                );
            });
        }

        m_sourceAndDestinationFolderBindPath.clear();
    }

    auto endIter = threadP.end();
    for(auto iter = threadP.begin(); iter != endIter; ++iter)
    {
        iter->join();
    }
}

void AsynchronousCopier::start(const IgnoreParser& ignoreParser, AsynchronousCopier::IndependentRun)
{
    std::vector<std::thread> threadP;

    {
        std::lock_guard<std::mutex> autoLocker{ m_mutex };
        
        threadP.reserve(m_sourceAndDestinationFileBindPath.empty() ? m_sourceAndDestinationFolderBindPath.size() : (m_sourceAndDestinationFolderBindPath.size() + 1));

        BindingPathArray arrayBind;
        std::swap(arrayBind, m_sourceAndDestinationFileBindPath);

        threadP.emplace_back(
            [
                fileCopyBindArray = std::move(arrayBind),
                toIgnoreData = ignoreParser
            ]()
        {
            auto endIter = fileCopyBindArray.end();
            for(auto iter = fileCopyBindArray.begin(); iter != endIter; ++iter)
            {
                AsynchronousCopier::internalCreateDestinationDirIfDoesntExists(iter->second, true);
                AsynchronousCopier::copyFile(iter->first, iter->second, toIgnoreData.getAllExtensionToIgnore());
            }
        });

        auto endIter = m_sourceAndDestinationFolderBindPath.end();
        for(auto iter = m_sourceAndDestinationFolderBindPath.begin(); iter != endIter; ++iter)
        {
            threadP.emplace_back(
                [
                    source = std::move(iter->first),
                    destination = std::move(iter->second),
                    toIgnoreData = ignoreParser
                ]() 
            {
                AsynchronousCopier::internalCreateDestinationDirIfDoesntExists(destination, false);

                AsynchronousCopier::copyRelevantDirectoryRecursive(
                    destination,
                    std::experimental::filesystem::directory_iterator{ source },
                    toIgnoreData.getAllExtensionToIgnore()
                );
            });
        }
        
        m_sourceAndDestinationFolderBindPath.clear();
    }

    auto endIter = threadP.end();
    for(auto iter = threadP.begin(); iter != endIter; ++iter)
    {
        iter->detach();
    }
}
