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
    m_Device.initialize(
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
    m_Device.clearView();

    m_Device.initView();

    RenderSceneManager::instance().update();

    m_Device.unbindTargetView();

    m_Device.presentToDisplay();
}

void RenderEngineManager::createCamera(const CameraParameters& cameraParameters)
{
    RenderSceneManager::instance().createCamera(cameraParameters);
}

void RenderEngineManager::setMainCameraMatViewManually(const DirectX::XMVECTOR& eyePosition, const DirectX::XMVECTOR& focusDirection, const DirectX::XMVECTOR& upDirection)
{
    RenderSceneManager::instance().setMainCameraMatViewManually(eyePosition, focusDirection, upDirection);
}
