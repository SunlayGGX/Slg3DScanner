#include "RenderEngineManager.h"

#include "EngineConfig.h"

#include "ShaderManager.h"
#include "RenderSceneManager.h"

#include "Camera.h"

using namespace Slg3DScanner;


RenderEngineManager::RenderEngineManager()
{

}

RenderEngineManager::~RenderEngineManager()
{

}

void RenderEngineManager::initialize()
{
    ShaderManager::instance().initialize();
    RenderSceneManager::instance().initialize();
}

void RenderEngineManager::initializeDevice(HWND windowVisuHandle)
{
    m_device.initialize(
        static_cast<Slg3DScanner::WindowsMode>(Slg3DScanner::Constants::WINDOWS_CONFIG_MODALITY),
        windowVisuHandle
    );
}

void RenderEngineManager::destroy()
{
    RenderSceneManager::instance().destroy();
    ShaderManager::instance().destroy();
}

void RenderEngineManager::update()
{
    m_device.clearView();

    m_device.initView();

    RenderSceneManager::instance().update();

    m_device.update();

    m_device.unbindTargetView();

    m_device.presentToDisplay();
}

void RenderEngineManager::createCamera(const CameraParameters& cameraParameters)
{
    RenderSceneManager::instance().createCamera(cameraParameters);
}

void RenderEngineManager::setMainCameraMatViewManually(const DirectX::XMVECTOR& eyePosition, const DirectX::XMVECTOR& focusDirection, const DirectX::XMVECTOR& upDirection)
{
    RenderSceneManager::instance().setMainCameraMatViewManually(eyePosition, focusDirection, upDirection);
}

void RenderEngineManager::getMainCameraMatViewManually(DirectX::XMVECTOR& outEyePosition, DirectX::XMVECTOR& outFocusDirection, DirectX::XMVECTOR& outUpDirection) const
{
    RenderSceneManager::instance().getMainCameraMatViewManually(outEyePosition, outFocusDirection, outUpDirection);
}

void RenderEngineManager::addDeviceAction(DXDispositif::Action action)
{
    m_device.addDispositifAction(action);
}
