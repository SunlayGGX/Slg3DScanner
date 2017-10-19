#include "Slg3DScannerPCH.h"

#include "Slg3DScannerConfig.h"

#include "Slg3DScannerApplication.h"
#include "WindowsPop.h"


int main(int argc, char** argv) try
{
    Slg3DScanner::Slg3DScannerApplication{ argc, argv }.run();
    
    return static_cast<int>(Slg3DScanner::ApplicationResult::SUCCESS);
}
catch(const std::exception& e)
{
    Slg3DScanner::WindowsPop::popErrorWindow(e.what());
    
    return static_cast<int>(Slg3DScanner::ApplicationResult::STANDARD_ERROR);
}
catch(...)
{
    Slg3DScanner::WindowsPop::popErrorWindow(SLG_NORMALIZE_EXCEPTION_MESSAGE("Unknown error"));

    return static_cast<int>(Slg3DScanner::ApplicationResult::UNKNOWN_ERROR);
}
