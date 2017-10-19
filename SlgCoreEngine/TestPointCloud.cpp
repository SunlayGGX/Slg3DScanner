#include "TesterManager.h"

#include "TestSheet1.h"

#include "DefaultObjectCreator.h"
#include "LoggerCommand.h"

#include "RenderEngineManager.h"

#include "CameraParameters.h"
#include "CloudMesh.h"

using namespace Slg3DScanner;


void TesterManager::test(SLG_TEST_ORDER_MAKE(LoadPointCloud))
{
    RenderEngineManager& renderMgr = RenderEngineManager::instance();
    auto& dxDevice = renderMgr.getDevice();

    CameraParameters cameraParameter;
    cameraParameter.aspectRatio = dxDevice.getScreenWidth() / dxDevice.getScreenHeight();

    SLGENGINE_LOG_DEBUG("Create camera");

    renderMgr.createCamera(cameraParameter);

    renderMgr.setMainCameraMatViewManually(
        testOrder.cameraPosition,
        testOrder.cameraDirection,
        testOrder.cameraUp
    );

    SLGENGINE_LOG_DEBUG("Read point cloud");

    DefaultObjectCreator::createDefaultPointCloud(testOrder.pointCloudFilePath);

    SLG_PRINT_END_TEST(LoadPointCloud);
}
