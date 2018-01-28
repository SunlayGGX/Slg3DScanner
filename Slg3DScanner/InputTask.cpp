#include "Slg3DScannerPCH.h"
#include "InputTask.h"

#include "InputEngine.h"
#include "RenderEngineManager.h"

#include "KeyboardElement.h"

using namespace Slg3DScanner;

InputTask::InputTask() :
    m_translationMove{ 0.01f },
    m_rotationMove{ 0.01f }
{
    
}

void InputTask::initialize()
{

}

void InputTask::destroy()
{

}

void InputTask::update()
{
    DirectX::XMVECTOR pos;
    DirectX::XMVECTOR dir;
    DirectX::XMVECTOR up;

    RenderEngineManager& renderMgr = RenderEngineManager::instance();
    renderMgr.getMainCameraMatViewManually(pos, dir, up);
    
    Slg3DScanner::KeyboardElement currentState = InputEngine::instance().getCurrentKeyboardState();
    Slg3DScanner::KeyboardElement currentDownState = InputEngine::instance().getKeyboardDownState();

    bool hasMoved = false;

    if(currentState.getKey(DIK_RIGHTARROW))
    {
        DirectX::XMVECTOR right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(dir, up));
        pos.m128_f32[0] += right.m128_f32[0] * m_translationMove;
        pos.m128_f32[1] += right.m128_f32[1] * m_translationMove;
        pos.m128_f32[2] += right.m128_f32[2] * m_translationMove;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_LEFTARROW))
    {
        DirectX::XMVECTOR right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(dir, up));
        pos.m128_f32[0] -= right.m128_f32[0] * m_translationMove;
        pos.m128_f32[1] -= right.m128_f32[1] * m_translationMove;
        pos.m128_f32[2] -= right.m128_f32[2] * m_translationMove;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_UPARROW))
    {
        pos.m128_f32[0] += dir.m128_f32[0] * m_translationMove;
        pos.m128_f32[1] += dir.m128_f32[1] * m_translationMove;
        pos.m128_f32[2] += dir.m128_f32[2] * m_translationMove;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_DOWNARROW))
    {
        pos.m128_f32[0] -= dir.m128_f32[0] * m_translationMove;
        pos.m128_f32[1] -= dir.m128_f32[1] * m_translationMove;
        pos.m128_f32[2] -= dir.m128_f32[2] * m_translationMove;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_NUMPAD8) || currentState.getKey(DIK_8))
    {
        pos.m128_f32[0] += up.m128_f32[0] * m_translationMove;
        pos.m128_f32[1] += up.m128_f32[1] * m_translationMove;
        pos.m128_f32[2] += up.m128_f32[2] * m_translationMove;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_NUMPAD2) || currentState.getKey(DIK_2))
    {
        pos.m128_f32[0] -= up.m128_f32[0] * m_translationMove;
        pos.m128_f32[1] -= up.m128_f32[1] * m_translationMove;
        pos.m128_f32[2] -= up.m128_f32[2] * m_translationMove;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_J))
    {
        float angle = dir.m128_f32[1] + m_rotationMove;
        if(angle >= DirectX::XM_2PI)
        {
            angle -= DirectX::XM_2PI;
        }

        dir = DirectX::XMVector3Rotate(dir, DirectX::XMQuaternionRotationAxis({ 0.f, 1.f, 0.f, 0.f }, angle));
        hasMoved = true;
    }

    if(currentState.getKey(DIK_L))
    {
        float angle = dir.m128_f32[1] - m_rotationMove;
        if(angle < 0.f)
        {
            angle += DirectX::XM_2PI;
        }

        dir = DirectX::XMVector3Rotate(dir, DirectX::XMQuaternionRotationAxis({ 0.f, 1.f, 0.f, 0.f }, angle));
        hasMoved = true;
    }

    if(currentDownState.getKey(DIK_R))
    {
        float angle = dir.m128_f32[1] - DirectX::XM_PI;
        if(angle < 0.f)
        {
            angle += DirectX::XM_2PI;
        }

        dir = DirectX::XMVector3Rotate(dir, DirectX::XMQuaternionRotationAxis({ 0.f, 1.f, 0.f, 0.f }, angle));
        hasMoved = true;
    }

    if(hasMoved)
    {
        renderMgr.setMainCameraMatViewManually(pos, dir, up);
    }

    if(currentState.getKey(DIK_W))
    {
        renderMgr.addDeviceAction(DXDispositif::Action::SET_WIREFRAME);
    }

    if(currentState.getKey(DIK_X))
    {
        renderMgr.addDeviceAction(DXDispositif::Action::SET_CULLBACK);
    }

    if(currentState.getKey(DIK_C))
    {
        renderMgr.addDeviceAction(DXDispositif::Action::SET_NO_CULL);
    }

    if(currentState.getKey(DIK_NUMPAD0))
    {
        renderMgr.addDeviceAction(DXDispositif::Action::CAPTURE_DEPTH);
    }

    if(currentDownState.getKey(DIK_ADD) || currentDownState.getKey(DIK_NUMPADPLUS))
    {
        m_translationMove *= 2.f;
        m_rotationMove *= 2.f;
    }

    if(currentDownState.getKey(DIK_MINUS) || currentDownState.getKey(DIK_NUMPADMINUS))
    {
        m_translationMove /= 2.f;
        m_rotationMove /= 2.f;
    }
}
