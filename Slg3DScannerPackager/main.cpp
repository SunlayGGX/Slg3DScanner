#include "Slg3DScannerPackagerLogUtils.h"

#include "IgnoreParser.h"
#include "AsynchronousCopier.h"
#include "EnvironmentVariableManip.h"
#include "Zipper.h"

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
    LOG("Begin Binding and searching for paths");

    std::experimental::filesystem::path destinationFolderRootName = "Slg3DScanner";

    std::string slgDependEnvironmentVariable = Slg3DScanner::SlgTools::EnvironmentVariableManip::getEnvironmentVariableValue("SLG_DEPEND");
    std::string slgDevelEnvironmentVariable = Slg3DScanner::SlgTools::EnvironmentVariableManip::getEnvironmentVariableValue("SLG_DEVEL");

    std::experimental::filesystem::path destinationPath = std::experimental::filesystem::temp_directory_path() / "SLG_TEMP" / destinationFolderRootName;
    std::experimental::filesystem::path develPath = std::experimental::filesystem::path{ slgDevelEnvironmentVariable } / "Slg3DScanner";

    Slg3DScanner::Tools::AsynchronousCopier asyncCopier;

    asyncCopier.bindSourceToDestinationPath(slgDependEnvironmentVariable, destinationPath / "SlgDependencies", false);
    asyncCopier.bindSourceToDestinationPath(develPath, destinationPath / "Developper" / "Slg3DScanner", false);
    asyncCopier.bindSourceToDestinationPath(develPath / "x64" / "Debug" / "Uninstaller_d.exe", destinationPath, true);
    asyncCopier.bindSourceToDestinationPath(develPath / "x64" / "Debug" / "Installer_d.exe", destinationPath, true);
    asyncCopier.bindSourceToDestinationPath(develPath / "README.txt", destinationPath, true);

    LOG("********************************\n\n");

    LOG("********************************");
    LOG("Begin Copying Relevant Files");

    asyncCopier.start<Slg3DScanner::Tools::AsynchronousCopier::WaitForCompletion>(ignoreParser);

    LOG("********************************\n\n");

    LOG("********************************");
    LOG("Zip folder begin");

    std::experimental::filesystem::path zipFolder = (destinationPath.parent_path() / destinationFolderRootName) += ".zip";
    if(Slg3DScanner::Tools::Zipper::zipFolder(destinationPath, zipFolder))
    {
        LOG("SUCCESS : Zip folder " + zipFolder.string() + " was successfully created from " + destinationPath.string());
    }
    else
    {
        LOG("FAILED : Zip folder " + zipFolder.string() + " wasn't created. Maybe your PowerShell version is too old (need 3.0 or later) or your .Net framework is too low (need 4.0 or higher)");
    }

    LOG("********************************\n\n");


    LOG("***********************************************************************");
    LOG("*****************************   END   *********************************");
    LOG("***********************************************************************");

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