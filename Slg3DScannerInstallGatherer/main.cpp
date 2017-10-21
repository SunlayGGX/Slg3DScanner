#include "Slg3DScannerInstallGathererLogUtils.h"

#include "IgnoreParser.h"
#include "AsynchronousCopier.h"
#include "EnvironmentVariableManip.h"

int main() try
{
    LOG("********************************");
    LOG("Load ignore file");
    
    Slg3DScanner::Tools::IgnoreParser ignoreParser;

    try
    {
        ignoreParser.load("extensionIgnore.txt");
        LOG("Ignore file successfully loaded");
    }
    catch(const std::exception& e)
    {
        LOG(("Cancel operation due to : " + std::string{ e.what() }).c_str());
    }
    LOG("********************************\n\n");
    
    LOG("********************************");
    LOG("Begin Copying Relevant Files");

    std::string slgDependEnvironmentVariable = Slg3DScanner::SlgTools::EnvironmentVariableManip::getEnvironmentVariableValue("SLG_DEPEND");
    std::string slgDevelEnvironmentVariable = Slg3DScanner::SlgTools::EnvironmentVariableManip::getEnvironmentVariableValue("SLG_DEVEL");

    std::experimental::filesystem::path destinationPath = std::experimental::filesystem::temp_directory_path() / "SLG_TEMP" / "Slg3DScanner";
    std::experimental::filesystem::path develPath = slgDevelEnvironmentVariable;

    Slg3DScanner::Tools::AsynchronousCopier asyncCopier;

    asyncCopier.bindSourceToDestinationPath(slgDependEnvironmentVariable, destinationPath / "SlgDependencies", false);
    asyncCopier.bindSourceToDestinationPath(slgDevelEnvironmentVariable, destinationPath / "Slg3DScanner", false);
    asyncCopier.bindSourceToDestinationPath(develPath / "Slg3DScanner" / "x64" / "Debug" / "Uninstaller_d.exe", destinationPath, true);
    asyncCopier.bindSourceToDestinationPath(develPath / "Slg3DScanner" / "x64" / "Debug" / "Installer_d.exe", destinationPath, true);

    asyncCopier.start<Slg3DScanner::Tools::AsynchronousCopier::WaitForCompletion>(ignoreParser);

    LOG("********************************\n\n");

    LOG("********************************");
    LOG("Copying Installer");
    asyncCopier.start<Slg3DScanner::Tools::AsynchronousCopier::WaitForCompletion>(ignoreParser);


    LOG("***********************************************************************");
    LOG("*****************************   END   *********************************");
    LOG("***********************************************************************");

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