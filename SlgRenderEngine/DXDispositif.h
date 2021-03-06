#pragma once

#include "UtilsMacros.h"

namespace Slg3DScanner
{
    enum class WindowsMode;

    class DXDispositif
    {
    public:
        const float BACKGROUND_DEFAULT_COLOR[4] = { 0.f, 0.5f, 0.f, 1.f };
        const float BLEND_FACTOR[4] = { 0.f, 0.f, 0.f, 0.f };


    private:
        ID3D11Device* m_D3DDevice;
        ID3D11DeviceContext* m_ImmediateContext;
        IDXGISwapChain* m_SwapChain;
        ID3D11RenderTargetView* m_RenderTargetView;

        ID3D11Texture2D* m_DepthTexture;
        ID3D11DepthStencilView* m_DepthStencilView;

        ID3D11RasterizerState* m_SolidCullBackRS;
        ID3D11RasterizerState* m_SolidCullNoneRS;
        ID3D11RasterizerState* m_WireframeCullNoneRS;

        ID3D11DepthStencilState* m_ZBufferEnable;
        ID3D11DepthStencilState* m_ZBufferDisable;

        ID3D11BlendState* m_AlphaBlendEnable;
        ID3D11BlendState* m_AlphaBlendDisable;

        float m_ScreenWidth;
        float m_ScreenHeight;

#ifdef _DEBUG_GRAPHIC
        ID3D11Debug* m_DXDebugDevice;
#endif


    public:
        DXDispositif();
        ~DXDispositif();


    public:
        void initialize(const WindowsMode& windowsMode, const HWND hWnd);

        void setWireFrameState();
        void setSolidCullNoneState();
        void setSolidCullBackState();

        void setEnableZBuffer(bool enable);
        void setEnableBlendAlpha(bool enable);

        DECLARE_SIMPLE_GET_ACCESSOR(ID3D11Device*,           D3DDevice);
        DECLARE_SIMPLE_GET_ACCESSOR(ID3D11DeviceContext*,    ImmediateContext);
        DECLARE_SIMPLE_GET_ACCESSOR(IDXGISwapChain*,         SwapChain);
        DECLARE_SIMPLE_GET_ACCESSOR(ID3D11RenderTargetView*, RenderTargetView);
        DECLARE_SIMPLE_GET_ACCESSOR(ID3D11Texture2D*,        DepthTexture);
        DECLARE_SIMPLE_GET_ACCESSOR(ID3D11DepthStencilView*, DepthStencilView);
        DECLARE_SIMPLE_GET_ACCESSOR(float, ScreenWidth);
        DECLARE_SIMPLE_GET_ACCESSOR(float, ScreenHeight);


    public:
        void clearView();

        void initView();

        void presentToDisplay();

        void unbindTargetView();

        //Debug
    public:
        void reportLiveObject();


    private:
        void internalCreateRenderView();
        void internalInitializeDepthBuffer();

        void internalConfigureStates();

        void internalInitializeDebugDevice();

        void internalConfigureImmediateContextToDefault();

        void internalInitializeViewPort();
    };
}

