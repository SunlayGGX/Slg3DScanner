#include "TesterManager.h"

#include "TestSheet1.h"

#include "RenderEngineManager.h"

#include "LoggerCommand.h"

using namespace Slg3DScanner;


void TesterManager::test(SLG_TEST_ORDER_MAKE(WriteAllPointCloudToObj))
{
    RenderEngineManager::instance().askWritePointCloudToObj(testOrder.objFolderPath);

    SLG_PRINT_END_TEST(WriteAllPointCloudToObj);
}
