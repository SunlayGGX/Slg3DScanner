#pragma once

namespace Slg3DScanner
{
    namespace SlgTools
    {
        class EnvironmentVariableManip
        {
        private:
            EnvironmentVariableManip() = delete;
            ~EnvironmentVariableManip() = delete;
            EnvironmentVariableManip(EnvironmentVariableManip&) = delete;
            EnvironmentVariableManip& operator=(EnvironmentVariableManip&) = delete;


        public:
            /*
            For use on Windows
            */
            static bool isValidEnvironmentVariableValue(const std::string& variableValue);

            /*
            For use on Windows
            Permanently set a user environment variable to the specified value. Create it if it doesn't exists.
            */
            static void setUserEnvironmentVariable(const std::string& variableName, const std::string& variableValue);

            /*
            For use on Windows
            Permanently set a user environment variable to the specified value. Create it if it doesn't exists.
            Keep in mind that if this function is used. The application won't work unless run with administrator privilege.
            */
            static void setSystemEnvironmentVariable(const std::string& variableName, const std::string& variableValue);

            /*
            For use on Windows
            Permanently set a user environment variable to the specified value. Create it if it doesn't exists.
            */
            static void removeUserEnvironmentVariable(const std::string& variableName);

            /*
            For use on Windows
            Permanently set a user environment variable to the specified value. Create it if it doesn't exists.
            Keep in mind that if this function is used. The application won't work unless run with administrator privilege.
            */
            static void removeSystemEnvironmentVariable(const std::string& variableName);
        };
    }
}
