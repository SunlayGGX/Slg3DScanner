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
    constexpr const float DELTA_MOVE = 0.05f;

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

    if(hasMoved)
    {
        renderMgr.setMainCameraMatViewManually(pos, dir, up);
    }

    //buggy (not thread safe) don't have time to fix for now
    if(currentState.getKey(DIK_W))
    {
        renderMgr.getDevice().setWireFrameState();
    }

    if(currentState.getKey(DIK_X))
    {
        renderMgr.getDevice().setSolidCullBackState();
    }

    if(currentState.getKey(DIK_C))
    {
        renderMgr.getDevice().setSolidCullNoneState();
    }
}
