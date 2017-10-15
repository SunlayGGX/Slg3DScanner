#pragma once

#include "UtilsMacros.h"

namespace Slg3DScanner
{
    class DXHardwareDeviceInfo
    {
    public:
        enum
        {
            CARD_NAME_MAX_LENGHT = 128
        };


    private:
        bool m_Valid;
        int m_Width;
        int m_Height;
        uint64_t m_Memory;
        wchar_t m_CardName[DXHardwareDeviceInfo::CARD_NAME_MAX_LENGHT];
        DXGI_MODE_DESC m_Mode;


    public:
        DXHardwareDeviceInfo(const DXGI_MODE_DESC& wantedDesc);
        ~DXHardwareDeviceInfo();


    public:
        DECLARE_SIMPLE_GET_ACCESSOR(bool, Valid)
        DECLARE_SIMPLE_GET_ACCESSOR(int, Width)
        DECLARE_SIMPLE_GET_ACCESSOR(int, Height)
        DECLARE_SIMPLE_GET_ACCESSOR(uint64_t, Memory)
        DECLARE_SIMPLE_GET_ACCESSOR(const wchar_t*, CardName)
        DECLARE_SIMPLE_GET_REFERENCE_ACCESSOR(DXGI_MODE_DESC, Mode)
    };
}

