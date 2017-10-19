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

    if(currentState.getKey(DIK_RIGHTARROW))
    {
        pos.m128_f32[0] += 0.1f;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_LEFTARROW))
    {
        pos.m128_f32[0] -= 0.1f;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_UPARROW))
    {
        pos.m128_f32[1] += 0.1f;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_DOWNARROW))
    {
        pos.m128_f32[1] -= 0.1f;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_8))
    {
        pos.m128_f32[2] += 0.1f;
        hasMoved = true;
    }

    if(currentState.getKey(DIK_2))
    {
        pos.m128_f32[2] -= 0.1f;
        hasMoved = true;
    }

    if(hasMoved)
    {
        renderMgr.setMainCameraMatViewManually(pos, dir, up);
    }
}
