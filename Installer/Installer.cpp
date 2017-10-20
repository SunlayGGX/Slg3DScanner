#include "SlgDevEnvBind.h"

namespace Slg3DScanner
{
    namespace SlgTools
    {
        void logInstallationFailed()
        {
            LOG("\n\n****************");
            LOG("Cancellation complete!");
            LOG("\nInstallation failed!\n");
        }

        void logInstallationSuccess()
        {
            LOG("\n\n****************");
            LOG("Operation successful!");
            LOG("\nInstallation complete!\n");
        }
    }
}

int main() try
{
    LOG("*******************************************");
    LOG("*** SETTING ENVIRONMENT VARIABLES PATHS ***");
    LOG("*******************************************");

    Slg3DScanner::SlgTools::linkSlgEnvironmentVariableToFolder(
        SLG_ENVIRONMENT_VARIABLE_BIND(USER, "SLG_DEVEL", "Slg3DScanner"),
        SLG_ENVIRONMENT_VARIABLE_BIND(USER, "SLG_DEPEND", "SlgDependencies")
    );

    Slg3DScanner::SlgTools::logInstallationSuccess();

    system("pause");
    return 0;
}
catch(const std::exception &e)
{
    Slg3DScanner::SlgTools::logInstallationFailed();

    ::MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
    return -99;
}
catch(...)
{
    Slg3DScanner::SlgTools::logInstallationFailed();

    ::MessageBoxA(nullptr, "Unknown error", "Error", MB_ICONERROR);
    return -100;
}
