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
            //Don't have time to do it clean
            std::string message;
            message.reserve(1024);

            message += "\nLoad a file named 'positionImage.slgBinPos2' in SlgData/PointCloud/ folder. Rename the model you chose to load like this\n";
            message += "To modify the triangle elimination threshold value, go to EngineConfig.h and modify THRESHOLD_ABERRANT_TRIANGLE constant and recompile. Sorry\n";
            message += "<- arrow to move right\n";
            message += "-> arrow to move left\n";
            message += "up arrow to move up\n";
            message += "down arrow to move down\n";
            message += "8 to move forward\n";
            message += "2 to move backward\n";
            message += "J to rotate to the left\n";
            message += "L to rotate to the right\n";
            message += "R to do a 180° rotation\n";
            message += "press '+' n time to move 2^n time faster\n";
            message += "press '-' n time to move 2^n time slower\n";
            message += "press 'Z' (azerty) or 'W' (qwerty) to go in wireframe mode\n";
            message += "press 'X' to go in cull back mode\n";
            message += "press 'C' to go in no cull mode\n";

            LoggerCommand::logOnConsole(message, Slg3DScanner::LoggerLevel::LEVEL_DEBUG);

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
