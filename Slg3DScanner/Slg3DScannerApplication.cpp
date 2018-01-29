#include "Slg3DScannerPCH.h"
#include "Slg3DScannerApplication.h"

#include "GlobalEngine.h"

#include "Slg3DScannerEngineComponentInstaller.h"

using Slg3DScanner::Slg3DScannerApplication;

Slg3DScannerApplication::Slg3DScannerApplication(int argc, char** argv)
{
    Slg3DScanner::autoAllocateAndInstallSlgSingleton(argc, argv);
}

Slg3DScannerApplication::~Slg3DScannerApplication()
{

}

void Slg3DScannerApplication::run()
{
    GlobalEngine::instance().run();
}
