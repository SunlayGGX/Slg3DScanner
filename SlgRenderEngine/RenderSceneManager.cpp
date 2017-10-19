#include "RenderSceneManager.h"

#include "LookPtr.h"

#include "RenderEngineManager.h"
#include "TimeManager.h"

#include "IMesh.h"
#include "Camera.h"

#include "ShaderConstantBuffer.h"


using namespace Slg3DScanner;


RenderSceneManager::RenderSceneManager()
{
    m_meshArray.reserve(16);
}

RenderSceneManager::~RenderSceneManager()
{
    //prevent crash when destroying RenderSceneManager before exiting render thread
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
}

void RenderSceneManager::initialize()
{

}

void RenderSceneManager::destroy()
{
    this->destroyCamera();

    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    m_meshArray.clear();
}

void RenderSceneManager::update()
{
    ID3D11DeviceContext* immediateContext = RenderEngineManager::instance().getDevice().getImmediateContext();

    float updateDeltaTime = TimeManager::instance().getLastFrameDurationInSeconds();
    
    this->updateCamera(updateDeltaTime);

    PreInitializeCBufferParameterFromRendererSceneManager preInitShadingCBuffer;

    preInitShadingCBuffer.m_CameraPos = this->getMainCameraPosition();
    preInitShadingCBuffer.m_LightVect = DirectX::XMVECTOR{ 0.f, -1.f, 0.f, 0.f };
    preInitShadingCBuffer.m_TransposedMatViewProj = this->getMainCameraTransposedMatViewProj();

    {
        std::lock_guard<std::mutex> autoLocker{ m_mutex };

        auto iterEnd = m_meshArray.end();
        for(auto iter = m_meshArray.begin(); iter != iterEnd; ++iter)
        {
            IMeshRef& mesh = (*iter);
            mesh->update(updateDeltaTime);
            mesh->draw(immediateContext, preInitShadingCBuffer);
        }
    }
}

void RenderSceneManager::updateCamera(float deltaTime)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    if(m_currentPlayerCamera)
    {
        m_currentPlayerCamera->update(deltaTime);
    }
}

void RenderSceneManager::createCamera(const CameraParameters& cameraParameters)
{
    CameraRef newCamera = std::make_shared<Camera>(cameraParameters);
    this->registerMainCamera(std::move(newCamera));
}

void RenderSceneManager::destroyCamera()
{
    this->unregisterMainCamera();
}

void RenderSceneManager::registerMainCamera(CameraRef newMainCamera)
{
    assert(newMainCamera);

    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    if(m_currentPlayerCamera != newMainCamera)
    {
        if(m_currentPlayerCamera)
        {
            m_currentPlayerCamera->m_isRegistered = false;
        }
        m_currentPlayerCamera = std::move(newMainCamera);
        m_currentPlayerCamera->m_isRegistered = true;
    }
}

void RenderSceneManager::unregisterMainCamera()
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    if(m_currentPlayerCamera)
    {
        m_currentPlayerCamera->m_isRegistered = false;
        m_currentPlayerCamera.reset();
    }
}

DirectX::XMVECTOR RenderSceneManager::getMainCameraPosition() const
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    if(m_currentPlayerCamera)
    {
        return m_currentPlayerCamera->getPosition();
    }

    return DirectX::XMVECTOR{ 0.f, 0.f, 0.f, 0.f };
}

DirectX::XMMATRIX RenderSceneManager::getMainCameraTransposedMatViewProj() const
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    if(m_currentPlayerCamera)
    {
        return m_currentPlayerCamera->getTransposedMatViewProj();
    }

    return DirectX::XMMatrixIdentity();
}

void RenderSceneManager::setMainCameraMatViewManually(const DirectX::XMVECTOR& eyePosition, const DirectX::XMVECTOR& focusDirection, const DirectX::XMVECTOR& upDirection)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    if(m_currentPlayerCamera)
    {
        m_currentPlayerCamera->setPosition(eyePosition);
        m_currentPlayerCamera->setDirection(focusDirection);
        m_currentPlayerCamera->setUp(upDirection);
        m_currentPlayerCamera->setMatView(DirectX::XMMatrixLookAtRH(
            eyePosition,
            focusDirection,
            upDirection
        ));
    }
}

void RenderSceneManager::getMainCameraMatViewManually(DirectX::XMVECTOR& outEyePosition, DirectX::XMVECTOR& outFocusDirection, DirectX::XMVECTOR& outUpDirection) const
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    if(m_currentPlayerCamera)
    {
        outEyePosition = m_currentPlayerCamera->getPosition();
        outFocusDirection = m_currentPlayerCamera->getDirection();
        outUpDirection = m_currentPlayerCamera->getUp();
    }
}

IMeshRef RenderSceneManager::findMeshByName(const std::string& meshName) const
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };

    auto endIter = m_meshArray.end();
    for(auto iter = m_meshArray.begin(); iter != endIter; ++iter)
    {
        if((*iter)->m_name == meshName)
        {
            return (*iter);
        }
    }

    return IMeshRef{};
}

std::size_t RenderSceneManager::internalFindIndexOfMeshByName(const std::string& meshName) const
{
    std::size_t endIndex = m_meshArray.size();
    for(std::size_t iter = 0; iter != endIndex; ++iter)
    {
        auto& meshRef = m_meshArray[iter];
        if(meshRef->m_name == meshName)
        {
            return iter;
        }
    }

    return endIndex;
}
