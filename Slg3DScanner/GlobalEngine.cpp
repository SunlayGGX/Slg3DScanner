#include "GlobalEngine.h"

#include "BitHelper.h"

#include "InputEngineMediator.h"
#include "SlgWindowMediator.h"
#include "LoggerEngine.h"
#include "RenderEngineManager.h"
#include "TimeManager.h"

#include "Slg3DScannerConfig.h"

#include "CameraParameters.h"

#include "DefaultObjectCreator.h"

#include "IMesh.h"

using namespace Slg3DScanner;


GlobalEngine::GlobalEngine() :
    m_run{ false }
{

}

GlobalEngine::~GlobalEngine()
{

}

void GlobalEngine::initialize()
{
    //RenderEngine::instance().initialize();
    if(m_run)
    {
        return;
    }

    m_run = true;

    this->startInputAndWindowsThread();

    TimeManager::instance().initialize();
}

void GlobalEngine::destroy()
{
    this->quit();

    TimeManager::instance().destroy();
    //RenderEngine::instance().destroy();
}

void GlobalEngine::run()
{
    constexpr const std::chrono::milliseconds updateTime{ 1000 / static_cast<int>(Slg3DScanner::Config::APPLICATION_FPS) };

    while(m_run && !this->isFullyInitialized())
    {
        std::this_thread::sleep_for(updateTime);
    }

    while(m_run)
    {
        /*TODO : Update the Game Loop*/

        //RenderEngine::instance().update();

        //Bad, but until we have a proper TimeManager, this will do the job...
        //TODO : Call a proper method from a Time Manager or Synchronize with VSync from RenderEngine swap chain present (present1 method) or so
        std::this_thread::sleep_for(updateTime);
    }
}

void GlobalEngine::quit()
{
    m_run = false;
}

void GlobalEngine::startInputAndWindowsThread() const
{
    std::thread
    {
        [&run = this->m_run]() 
    {
        SlgWindowMediator windowMgrMediator(&LoggerEngine::instance());
        InputEngineMediator inputMgrMediator(&LoggerEngine::instance(), windowMgrMediator.getWindowHandle());

        while(run)
        {
            inputMgrMediator.update();
            windowMgrMediator.update();
            TimeManager::instance().waitEndOfFrame();
        }
    }}.detach();
}

void GlobalEngine::startRendering(HWND windowVisuHandle)
{
    RenderEngineManager& renderMgr = RenderEngineManager::instance();

    renderMgr.initializeDevice(windowVisuHandle);
    renderMgr.initialize();
    
    auto& dxDevice = renderMgr.getDevice();

    m_allInitialized |= BitMask<GlobalEngine::RENDER_INITIALIZED>::value;
    CameraParameters cameraParameter;
    cameraParameter.aspectRatio = dxDevice.getScreenWidth() / dxDevice.getScreenHeight();

    renderMgr.createCamera(cameraParameter);

    this->arrangeObjectInSceneWorld();

    this->internalStartRenderThread();
}

void GlobalEngine::arrangeObjectInSceneWorld()
{
    RenderEngineManager::instance().setMainCameraMatViewManually(
        DirectX::XMVectorSet(0.f, 0.f, -10.f, 1.f),
        DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f),
        DirectX::XMVectorSet(0.f, 1.f, 0.f, 1.f)
    );

    auto cube = DefaultObjectCreator::createDefaultCubeMesh();
    cube->getMeshParams().m_TransposedMatWorld = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-2.f, 2.f, -2.f)) * cube->getMeshParams().m_TransposedMatWorld;

    auto cube2 = DefaultObjectCreator::createDefaultCubeMesh();
    cube2->getMeshParams().m_TransposedMatWorld = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-2.f, -2.f, -2.f)) * cube2->getMeshParams().m_TransposedMatWorld;

    auto cube3 = DefaultObjectCreator::createDefaultCubeMesh();
    cube3->getMeshParams().m_TransposedMatWorld = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(2.f, 2.f, -2.f)) * cube3->getMeshParams().m_TransposedMatWorld;

    auto cube4 = DefaultObjectCreator::createDefaultCubeMesh();
    cube4->getMeshParams().m_TransposedMatWorld = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(2.f, -2.f, -2.f)) * cube4->getMeshParams().m_TransposedMatWorld;
}

void GlobalEngine::internalStartRenderThread() const
{
    std::thread
    {
        [&run = this->m_run]()
    {
        RenderEngineManager& renderMgr = RenderEngineManager::instance();
        while(run)
        {
            renderMgr.update();
            TimeManager::instance().waitEndOfFrame();
        }

        renderMgr.destroy();
    }}.detach();
}

bool GlobalEngine::isFullyInitialized() const
{
    return m_allInitialized == BitFill<GlobalEngine::FULL_INITIALIZED>::value;
}
