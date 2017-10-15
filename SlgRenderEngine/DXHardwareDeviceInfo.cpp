#include "DXHardwareDeviceInfo.h"

#include "DirectXUtilitary.h"

using namespace Slg3DScanner;


DXHardwareDeviceInfo::DXHardwareDeviceInfo(const DXGI_MODE_DESC& wantedDesc) :
    m_Valid{ false }
{
    IDXGIFactory* pFactory = nullptr;
    IDXGIAdapter* pAdapter = nullptr;
    IDXGIOutput* pOutput = nullptr;

    CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pFactory));

    if (FAILED(pFactory->EnumAdapters(0, &pAdapter)))
    {
        return;
    }

    if (FAILED(pAdapter->EnumOutputs(0, &pOutput)))
    {
        return;
    }

    { //current state info
        DXGI_OUTPUT_DESC outDesc;
        pOutput->GetDesc(&outDesc);

        m_Width = outDesc.DesktopCoordinates.right - outDesc.DesktopCoordinates.left;
        m_Height = outDesc.DesktopCoordinates.bottom - outDesc.DesktopCoordinates.top;

        m_Valid = true;

        pOutput->FindClosestMatchingMode(&wantedDesc, &m_Mode, nullptr);
    }

    DXGI_ADAPTER_DESC desc;
    pAdapter->GetDesc(&desc);

    m_Memory = static_cast<uint64_t>(desc.DedicatedVideoMemory);

    wcscpy_s(m_CardName, DXHardwareDeviceInfo::CARD_NAME_MAX_LENGHT, desc.Description);

    DXRelease(
        pOutput,
        pAdapter,
        pFactory
    );
}

DXHardwareDeviceInfo::~DXHardwareDeviceInfo() = default;
