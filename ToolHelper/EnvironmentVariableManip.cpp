#include "ToolHelperPCH.h"
#include "EnvironmentVariableManip.h"

namespace Slg3DScanner
{
    namespace SlgTools
    {
        bool EnvironmentVariableManip::isValidEnvironmentVariableValue(const std::string& variableValue)
        {
            return variableValue.find(' ') == std::string::npos;
        }

        void EnvironmentVariableManip::setUserEnvironmentVariable(const std::string& variableName, const std::string& variableValue)
        {
            if(EnvironmentVariableManip::isValidEnvironmentVariableValue(variableValue))
            {
                system(("setx " + variableName + " " + variableValue).c_str());
            }
            else
            {
                system(("setx " + variableName + " \"" + variableValue + "\"").c_str());
            }
        }

        void EnvironmentVariableManip::setSystemEnvironmentVariable(const std::string& variableName, const std::string& variableValue)
        {
            if(EnvironmentVariableManip::isValidEnvironmentVariableValue(variableValue))
            {
                system(("setx -m " + variableName + " " + variableValue).c_str());
            }
            else
            {
                system(("setx -m " + variableName + " \"" + variableValue + "\"").c_str());
            }
        }

        void EnvironmentVariableManip::removeUserEnvironmentVariable(const std::string& variableName)
        {
            system(("REG delete HKCU\\Environment /F /V " + variableName).c_str());
        }

        void EnvironmentVariableManip::removeSystemEnvironmentVariable(const std::string& variableName)
        {
            system(("REG delete \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\" /F /V " + variableName).c_str());
        }
    }
}
