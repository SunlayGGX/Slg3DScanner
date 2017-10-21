#include "SlgDevEnvUnbind.h"

#include "UninstallerLogUtils.h"

namespace Slg3DScanner
{
    namespace SlgTools
    {
        void logUninstallationSuccess()
        {
            LOG("\n\n****************");
            LOG("Operation successful!");
            LOG("\nUninstallation complete!\n");
        }
    }
}

int main() try
{
    LOG("********************************************");
    LOG("*** REMOVING ENVIRONMENT VARIABLES PATHS ***");
    LOG("********************************************");

    Slg3DScanner::SlgTools::unlinkSlgEnvironmentVariableToFolder(
        SLG_ENVIRONMENT_VARIABLE_UNBIND(USER, "SLG_DEVEL"),
        SLG_ENVIRONMENT_VARIABLE_UNBIND(USER, "SLG_DEPEND")
    );

    Slg3DScanner::SlgTools::logUninstallationSuccess();

    system("pause");
    return 0;
}
catch(const std::exception &e)
{
    ::MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
    return -99;
}
catch(...)
{
    ::MessageBoxA(nullptr, "Unknown error", "Error", MB_ICONERROR);
    return -100;
}
