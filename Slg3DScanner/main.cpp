#include "Slg3DScannerPCH.h"

#include "Slg3DScannerConfig.h"


int main(int argc, char** argv) try
{
    return static_cast<int>(Slg3DScanner::ApplicationResult::SUCCESS);
}
catch(const std::exception& e)
{
    return static_cast<int>(Slg3DScanner::ApplicationResult::STANDARD_ERROR);
}
catch(...)
{
    return static_cast<int>(Slg3DScanner::ApplicationResult::UNKNOWN_ERROR);
}
