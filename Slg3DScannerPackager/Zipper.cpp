#include "Slg3DScannerPackagerPCH.h"
#include "Zipper.h"

#include "CommandPrompt.h"

using namespace Slg3DScanner::Tools;

bool Zipper::zipFolder(const std::experimental::filesystem::path& structurePathToZip, const std::experimental::filesystem::path& structurePathToFlushZip)
{
    if(!std::experimental::filesystem::is_directory(structurePathToZip))
    {
        throw std::exception{ (structurePathToZip.string() + " is not a valid folder to zip").c_str() };
    }

    if(std::experimental::filesystem::exists(structurePathToFlushZip))
    {
        std::experimental::filesystem::remove(structurePathToFlushZip);
    }

    CommandPrompt commandPrompt;
    commandPrompt.addCommand(
        "powershell.exe -version 3 -nologo -noprofile -command \"& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::CreateFromDirectory('" + structurePathToZip.string() + "', '" + structurePathToFlushZip.string() + "'); }\"", 0);

    auto result = commandPrompt.executeCommand();

    return result.second.second == 0;
}
