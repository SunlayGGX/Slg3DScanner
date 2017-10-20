#include "Slg3DScannerInstallGathererLogUtils.h"


int main() try
{
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