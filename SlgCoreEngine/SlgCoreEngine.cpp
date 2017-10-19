#include "SlgCoreEnginePCH.h"
#include "SlgCoreEngine.h"

#include "TesterManager.h"

using namespace Slg3DScanner;

SlgCoreEngine::SlgCoreEngine()
{
}

SlgCoreEngine::~SlgCoreEngine()
{
}

void SlgCoreEngine::initialize()
{
    TesterManager::instance().initialize();
}

void SlgCoreEngine::destroy()
{
    TesterManager::instance().destroy();
}

void SlgCoreEngine::runTest()
{
    TesterManager::instance().startTestsQueue<
        SLG_TEST_ORDER(DrawCubes),
        SLG_TEST_ORDER(LoadPointCloud)
    >();
}
