#pragma once

#include "UtilsMacros.h"

namespace Slg3DScanner
{
    enum class WindowsMode;

    class DXDispositif
    {
    public:
        using DepthType = float;


    public:
        enum class Action
        {
            SET_WIREFRAME,
            SET_NO_CULL,
            SET_CULLBACK,
            
            ENABLE_Z_BUFFER,
            DISABLE_Z_BUFFER,

            ENABLE_BLEND_ALPHA,
            DISABLE_BLEND_ALPHA,

            CAPTURE_DEPTH
        };


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

        ID3D11Texture2D* m_DepthCapturedTexture;

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

        std::mutex m_actionMutex;
        std::vector<Action> m_actionQueue;

        mutable std::mutex m_depthCaptureMutex;
        std::unique_ptr<DepthType[]> m_depthCapturedData;
        std::size_t m_depthCapturedDataSizeCount;


    public:
        DXDispositif();
        ~DXDispositif();


    public:
        void initialize(const WindowsMode& windowsMode, const HWND hWnd);

        void update();

        void addDispositifAction(Action action);


    private:
        void internalExecuteAction(Action actionToExecute);


    private:
        void setWireFrameState();
        void setSolidCullNoneState();
        void setSolidCullBackState();

        void setEnableZBuffer(bool enable);
        void setEnableBlendAlpha(bool enable);

        void captureDepthBuffer();


    public:
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


    public:
        std::unique_ptr<DepthType[]> retrieveLastCapturedDepthBufferData(std::size_t& outDataSize) const;


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

