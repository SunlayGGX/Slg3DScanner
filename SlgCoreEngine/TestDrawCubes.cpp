#include "TesterManager.h"

#include "TestSheet1.h"

#include "DefaultObjectCreator.h"
#include "LoggerCommand.h"

#include "RenderEngineManager.h"

#include "CameraParameters.h"
#include "CubeMesh.h"

using namespace Slg3DScanner;

void TesterManager::test(SLG_TEST_ORDER_MAKE(DrawCubes))
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

    SLGENGINE_LOG_DEBUG("Create cube 1");
    auto cube = DefaultObjectCreator::createDefaultCubeMesh();
    cube->getMeshParams().m_TransposedMatWorld = 
        DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(testOrder.cube1Position.x, testOrder.cube1Position.y, testOrder.cube1Position.z)) * cube->getMeshParams().m_TransposedMatWorld;

    SLGENGINE_LOG_DEBUG("Create cube 2");
    auto cube2 = DefaultObjectCreator::createDefaultCubeMesh();
    cube2->getMeshParams().m_TransposedMatWorld = 
        DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(testOrder.cube2Position.x, testOrder.cube2Position.y, testOrder.cube2Position.z)) * cube2->getMeshParams().m_TransposedMatWorld;

    SLGENGINE_LOG_DEBUG("Create cube 3");
    auto cube3 = DefaultObjectCreator::createDefaultCubeMesh();
    cube3->getMeshParams().m_TransposedMatWorld = 
        DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(testOrder.cube3Position.x, testOrder.cube3Position.y, testOrder.cube3Position.z)) * cube3->getMeshParams().m_TransposedMatWorld;

    SLGENGINE_LOG_DEBUG("Create cube 4");
    auto cube4 = DefaultObjectCreator::createDefaultCubeMesh();
    cube4->getMeshParams().m_TransposedMatWorld = 
        DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(testOrder.cube4Position.x, testOrder.cube4Position.y, testOrder.cube4Position.z)) * cube4->getMeshParams().m_TransposedMatWorld;

    SLG_PRINT_END_TEST(DrawCubes);
}
