#include "Slg3DScannerPCH.h"

#include "Slg3DScannerConfig.h"

#include "WindowsPop.h"


int main(int argc, char** argv) try
{
    return static_cast<int>(Slg3DScanner::ApplicationResult::SUCCESS);
}
catch(const std::exception& e)
{
    Slg3DScanner::WindowsPop::popErrorWindow(e.what());
    return static_cast<int>(Slg3DScanner::ApplicationResult::STANDARD_ERROR);
}
catch(...)
{
    return static_cast<int>(Slg3DScanner::ApplicationResult::UNKNOWN_ERROR);
}
