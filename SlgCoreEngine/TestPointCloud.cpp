#include "TesterManager.h"

#include "TestSheet1.h"

#include "DefaultObjectCreator.h"
#include "LoggerCommand.h"

#include "CameraParameters.h"
#include "CloudMesh.h"

using namespace Slg3DScanner;

namespace Slg3DScanner
{
    namespace TestDetails
    {
        template<class TestType>
        void loadPointCloudTestHelper(const TestType& testOrder)
        {
            SLGENGINE_LOG_DEBUG("Create camera");
            DefaultObjectCreator::createDefaultCamera(testOrder.cameraPosition, testOrder.cameraDirection, testOrder.cameraUp);

            SLGENGINE_LOG_DEBUG("Read point cloud");
            DefaultObjectCreator::createDefaultPointCloud(testOrder.pointCloudFilePath, testOrder.version);
        };
    }
}


void TesterManager::test(SLG_TEST_ORDER_MAKE(LoadPointCloud))
{
    TestDetails::loadPointCloudTestHelper(testOrder);
    SLG_PRINT_END_TEST(LoadPointCloud);
}

void TesterManager::test(SLG_TEST_ORDER_MAKE(LoadPointCloudSimple))
{
    TestDetails::loadPointCloudTestHelper(testOrder);
    SLG_PRINT_END_TEST(LoadPointCloudSimple);
}

void TesterManager::test(SLG_TEST_ORDER_MAKE(LoadPointCloudTranslatedSimple))
{
    TestDetails::loadPointCloudTestHelper(testOrder);
    SLG_PRINT_END_TEST(LoadPointCloudTranslatedSimple);
}
