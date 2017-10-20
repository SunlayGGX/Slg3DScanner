#pragma once

#include "InstallerLogUtils.h"

#include "EnvironmentVariableManip.h"
#include "FolderFinder.h"

namespace Slg3DScanner
{
    namespace SlgTools
    {
        template<class LinkDelegate, class UnlinkDelegate, class StringType, class ... Args>
        void linkSlgEnvironmentVariableToFolder(LinkDelegate linkFunction, UnlinkDelegate unlinkFunction, const StringType& environmentVariableName, const StringType& folderName, const Args& ... otherQuadruplets)
        {
            linkSlgEnvironmentVariableToFolder(linkFunction, unlinkFunction, environmentVariableName, folderName);

            try
            {
                linkSlgEnvironmentVariableToFolder(otherQuadruplets...);
            }
            catch(const std::exception&)
            {
                LOG("\nRemoving environment variable " << environmentVariableName);
                unlinkFunction(environmentVariableName);
                throw;
            }
        }

        template<class LinkDelegate, class UnlinkDelegate, class StringType>
        void linkSlgEnvironmentVariableToFolder(LinkDelegate linkFunction, UnlinkDelegate unlinkFunction, const StringType& environmentVariableName, const StringType& folderName) try
        {
            LOG("\n\nsetting " << environmentVariableName << " environment variable to " << folderName << " folder");

            std::string folderToBind = Slg3DScanner::SlgTools::FolderFinder::findFolderPathFromCurrentPath(folderName);

            linkFunction(environmentVariableName, folderToBind);

            LOG("SUCCESS: Environment variable named " << environmentVariableName << " was created and set to " << folderToBind);
        }
        catch(const std::exception& e)
        {
            LOG("FAILED : setting " << environmentVariableName << " to " << folderName << " was aborted because : " << e.what());
            LOG("\n\n\n******************************");
            LOG("****** BEGIN CANCELLING ******");
            LOG("******************************\n");
            throw;
        }
    }
}

#define SLG_ENVIRONMENT_VARIABLE_BIND(modality, environmentVariableName, folderName) GET_LINK_ENVIR_METHOD(modality), GET_UNLINK_ENVIR_METHOD(modality), std::string{ environmentVariableName }, std::string{ folderName }
