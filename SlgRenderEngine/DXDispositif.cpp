#include "DXDispositif.h"

#include "EngineConfig.h"
#include "DirectXUtilitary.h"

#include "DXHardwareDeviceInfo.h"

using namespace Slg3DScanner;

DXDispositif::DXDispositif() :
    m_DepthTexture{ nullptr },
    m_DepthStencilView{ nullptr },
    m_SolidCullBackRS{ nullptr },
    m_SolidCullNoneRS{ nullptr },
    m_WireframeCullNoneRS{ nullptr },
    m_ZBufferEnable{ nullptr },
    m_ZBufferDisable{ nullptr },
    m_AlphaBlendEnable{ nullptr },
    m_AlphaBlendDisable{ nullptr },
    m_RenderTargetView{ nullptr },
    m_SwapChain{ nullptr },
    m_D3DDevice{ nullptr }
{

}

DXDispositif::~DXDispositif()
{
    DXRelease(
        m_DepthTexture,
        m_SolidCullBackRS,
        m_SolidCullNoneRS,
        m_WireframeCullNoneRS,
        m_ZBufferEnable,
        m_ZBufferDisable,
        m_AlphaBlendEnable,
        m_AlphaBlendDisable
    );

    this->unbindTargetView();

    if (m_ImmediateContext != nullptr)
    {
        m_ImmediateContext->ClearState();
        m_ImmediateContext->Release();
        m_ImmediateContext = nullptr;
    }

    if (m_SwapChain != nullptr)
    {
        //swap chain must be windowed if we want to release it.
        m_SwapChain->SetFullscreenState(FALSE, nullptr);
        m_SwapChain->Release();
    }

    DXRelease(
        m_RenderTargetView,
        m_DepthStencilView,
        m_D3DDevice
    );

#ifdef _DEBUG_GRAPHIC
    this->reportLiveObject();
    DXRelease(m_DXDebugDevice);
#endif // _DEBUG_GRAPHIC
}

void DXDispositif::initialize(const WindowsMode& windowsMode, const HWND hWnd)
{
    { //Create the device, the immediate context and the swap chain altogether
        
        //Swap chain mode initialization
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        DXZeroMemory(swapChainDesc);

        {
            DXGI_MODE_DESC descModeWanted;
            descModeWanted.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            descModeWanted.Height = static_cast<UINT>(WantedResolution::HEIGHT);
            descModeWanted.Width = static_cast<UINT>(WantedResolution::WIDTH);
            descModeWanted.RefreshRate.Numerator = static_cast<UINT>(Slg3DScanner::Constants::FPS);
            descModeWanted.RefreshRate.Denominator = 1;
            descModeWanted.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            descModeWanted.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

            DXHardwareDeviceInfo infos{ descModeWanted };

            swapChainDesc.BufferDesc.Format = infos.getMode().Format;

            swapChainDesc.BufferDesc.Width = infos.getMode().Width;
            swapChainDesc.BufferDesc.Height = infos.getMode().Height;

            m_ScreenWidth = static_cast<float>(swapChainDesc.BufferDesc.Width);
            m_ScreenHeight = static_cast<float>(swapChainDesc.BufferDesc.Height);
        }

        swapChainDesc.BufferDesc.RefreshRate.Numerator = static_cast<UINT>(Slg3DScanner::Constants::FPS);
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;

        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 1;

        swapChainDesc.OutputWindow = hWnd;

        swapChainDesc.Windowed = static_cast<BOOL>(windowsMode);

        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        {
            RECT rcClient;
            RECT rcWindow;
            GetClientRect(hWnd, &rcClient);
            GetWindowRect(hWnd, &rcWindow);

            POINT ptDiff{ (rcWindow.right - rcWindow.left) - rcClient.right, (rcWindow.bottom - rcWindow.top) - rcClient.bottom };

            MoveWindow(
                hWnd, 
                rcWindow.left, 
                rcWindow.top, 
                swapChainDesc.BufferDesc.Width + ptDiff.x, 
                swapChainDesc.BufferDesc.Height + ptDiff.y, 
                TRUE
            );
        }

        UINT deviceCreationFlags = 0;

#ifdef _DEBUG
        deviceCreationFlags |= D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif

        constexpr const D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
        };

        DXTry(D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            deviceCreationFlags,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &m_SwapChain,
            &m_D3DDevice,
            nullptr,
            &m_ImmediateContext
        ));
    }
    
    this->internalCreateRenderView();
    this->internalInitializeDepthBuffer();

    this->internalConfigureStates();

    this->internalInitializeDebugDevice();

    this->internalConfigureImmediateContextToDefault();

    this->internalInitializeViewPort();
}

void DXDispositif::internalCreateRenderView()
{
    ID3D11Texture2D* backBuffer;

    DXTry(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBuffer)));
    DXTry(m_D3DDevice->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView));
    
    backBuffer->Release();
}

void DXDispositif::internalInitializeDepthBuffer()
{
    D3D11_TEXTURE2D_DESC depthTextureDesc;
    DXZeroMemory(depthTextureDesc);

    depthTextureDesc.Width = static_cast<UINT>(m_ScreenWidth);
    depthTextureDesc.Height = static_cast<UINT>(m_ScreenHeight);
    depthTextureDesc.MipLevels = 1;
    depthTextureDesc.ArraySize = 1;
    depthTextureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthTextureDesc.SampleDesc.Count = 1;
    depthTextureDesc.SampleDesc.Quality = 0;
    depthTextureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    depthTextureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
    depthTextureDesc.CPUAccessFlags = 0;

    DXTry(m_D3DDevice->CreateTexture2D(&depthTextureDesc, nullptr, &m_DepthTexture));

    D3D11_DEPTH_STENCIL_VIEW_DESC descStencilView;
    DXZeroMemory(descStencilView);

    descStencilView.Format = depthTextureDesc.Format;
    descStencilView.ViewDimension =  D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
    descStencilView.Texture2D.MipSlice = 0;

    DXTry(m_D3DDevice->CreateDepthStencilView(m_DepthTexture, &descStencilView, &m_DepthStencilView));
}

void DXDispositif::internalConfigureStates()
{
    { //Blend State creation
        D3D11_BLEND_DESC blendDesc;
        DXZeroMemory(blendDesc);

        blendDesc.IndependentBlendEnable = FALSE;

        blendDesc.RenderTarget[0].BlendEnable = TRUE;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        DXTry(m_D3DDevice->CreateBlendState(&blendDesc, &m_AlphaBlendEnable));

        blendDesc.RenderTarget[0].BlendEnable = FALSE;

        DXTry(m_D3DDevice->CreateBlendState(&blendDesc, &m_AlphaBlendDisable));
    }

    this->setEnableBlendAlpha(true);

    { //Rasterizer state creation
        D3D11_RASTERIZER_DESC rsDesc;
        DXZeroMemory(rsDesc);

        rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        rsDesc.FrontCounterClockwise = FALSE;
        m_D3DDevice->CreateRasterizerState(&rsDesc, &m_SolidCullBackRS);

        rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

        DXTry(m_D3DDevice->CreateRasterizerState(&rsDesc, &m_SolidCullNoneRS));

        rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

        DXTry(m_D3DDevice->CreateRasterizerState(&rsDesc, &m_WireframeCullNoneRS));
    }

    this->setSolidCullBackState();

    // Z-Buffer State creation
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    DXZeroMemory(depthStencilDesc);

    depthStencilDesc.DepthEnable = FALSE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;

    DXTry(m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_ZBufferDisable));

    depthStencilDesc.DepthEnable = TRUE;

    DXTry(m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_ZBufferEnable));

    this->setEnableZBuffer(true);
}

void DXDispositif::internalInitializeDebugDevice()
{
#ifdef _DEBUG_GRAPHIC
    try
    {
        DXTry(m_D3DDevice->QueryInterface(&m_DXDebugDevice));
        //DXTry(m_Device->QueryInterface(IID_PPV_ARGS(&m_DXDebugDevice)));
    }
    catch (HRESULT resultException)
    {
        LOG_DEBUG(m_Logger, "Error : Failed to initialize dispositif debug\nError : " << resultException);
    }
#endif
}

void DXDispositif::internalConfigureImmediateContextToDefault()
{
    m_ImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DXDispositif::internalInitializeViewPort()
{
    D3D11_VIEWPORT viewPort;

    viewPort.Width = m_ScreenWidth;
    viewPort.Height = m_ScreenHeight;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;

    m_ImmediateContext->RSSetViewports(1, &viewPort);
}

void DXDispositif::setWireFrameState()
{
    m_ImmediateContext->RSSetState(m_WireframeCullNoneRS);
}

void DXDispositif::setSolidCullNoneState()
{
    m_ImmediateContext->RSSetState(m_SolidCullNoneRS);
}

void DXDispositif::setSolidCullBackState()
{
    m_ImmediateContext->RSSetState(m_SolidCullBackRS);
}

void DXDispositif::setEnableZBuffer(bool enable)
{
    m_ImmediateContext->OMSetDepthStencilState(enable ? m_ZBufferEnable : m_ZBufferDisable, 0);
}

void DXDispositif::setEnableBlendAlpha(bool enable)
{
    m_ImmediateContext->OMSetBlendState(enable ? m_AlphaBlendEnable : m_AlphaBlendDisable, BLEND_FACTOR, 0xFFFFFFFF);
}

void DXDispositif::clearView()
{
	m_ImmediateContext->ClearRenderTargetView(m_RenderTargetView, BACKGROUND_DEFAULT_COLOR);
	m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.f, 0);
}

void DXDispositif::unbindTargetView()
{
    ID3D11RenderTargetView* voidTargetView = nullptr;

    m_ImmediateContext->OMSetRenderTargets(1, &voidTargetView, nullptr);

    if(voidTargetView != nullptr)
    {
        voidTargetView->Release();
    }
}

void DXDispositif::initView()
{
    m_ImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
}

void DXDispositif::presentToDisplay()
{
    m_SwapChain->Present(0, 0);
}

void DXDispositif::reportLiveObject()
{
#ifdef _DEBUG_GRAPHIC
    if (m_DXDebugDevice != nullptr)
    {
        DXTry(m_DXDebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_DETAIL));
    }
#endif // _DEBUG
}
