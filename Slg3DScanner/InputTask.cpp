#include "Slg3DScannerPCH.h"
#include "InputTask.h"

#include "InputEngine.h"
#include "RenderEngineManager.h"

#include "KeyboardElement.h"

using namespace Slg3DScanner;

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

    bool hasMoved = false;
    constexpr const float DELTA_MOVE = 0.02f;
    constexpr const float DELTA_ROTATE = 0.02f;

    if(currentState.getKey(DIK_RIGHTARROW))
    {
        pos.m128_f32[0] += DELTA_MOVE;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_LEFTARROW))
    {
        pos.m128_f32[0] -= DELTA_MOVE;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_UPARROW))
    {
        pos.m128_f32[1] += DELTA_MOVE;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_DOWNARROW))
    {
        pos.m128_f32[1] -= DELTA_MOVE;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_NUMPAD8) || currentState.getKey(DIK_8))
    {
        pos.m128_f32[2] += DELTA_MOVE;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_NUMPAD2) || currentState.getKey(DIK_2))
    {
        pos.m128_f32[2] -= DELTA_MOVE;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_L))
    {
        float angle = dir.m128_f32[1] + DELTA_ROTATE;
        if(angle >= DirectX::XM_2PI)
        {
            angle -= DirectX::XM_2PI;
        }

        dir = DirectX::XMVector3Rotate(dir, DirectX::XMQuaternionRotationAxis({ 0.f, 1.f, 0.f, 0.f }, angle));
        hasMoved = true;
    }

    if(currentState.getKey(DIK_J))
    {
        float angle = dir.m128_f32[1] - DELTA_ROTATE;
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
}
