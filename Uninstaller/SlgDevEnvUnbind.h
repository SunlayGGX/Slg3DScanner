#pragma once

#include "UninstallerLogUtils.h"

#include "EnvironmentVariableManip.h"


namespace Slg3DScanner
{
    namespace SlgTools
    {
        template<class UnlinkDelegate, class StringType, class ... Args>
        void unlinkSlgEnvironmentVariableToFolder(UnlinkDelegate unlinkFunction, const StringType& environmentVariableName, const Args& ... otherPairs)
        {
            unlinkSlgEnvironmentVariableToFolder(unlinkFunction, environmentVariableName);
            unlinkSlgEnvironmentVariableToFolder(otherPairs...);
        }

        template<class UnlinkDelegate, class StringType>
        void unlinkSlgEnvironmentVariableToFolder(UnlinkDelegate unlinkFunction, const StringType& environmentVariableName)
        {
            LOG("Prepare to remove environment variable named : " << environmentVariableName);
            unlinkFunction(environmentVariableName);
        }
    }
}

#define SLG_ENVIRONMENT_VARIABLE_UNBIND(modality, environmentVariableName, folderName) GET_UNLINK_ENVIR_METHOD(modality), std::string{ environmentVariableName }
