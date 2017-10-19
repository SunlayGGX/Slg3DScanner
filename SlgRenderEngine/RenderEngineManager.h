#pragma once

#include "SlgSingleton.h"
#include "UtilsMacros.h"
#include "GlobalDef.h"

#include "DXDispositif.h"

#pragma warning(disable: 4251)

namespace Slg3DScanner
{
    struct CameraParameters;
    DECLARE_SLG_SINGLETON_ALLOCATOR_HELPER_FOR(RenderEngineManager)

    class RenderEngineManager : private SlgSingleton<RenderEngineManager>
    {
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(RenderEngineManager);


    private:
        DXDispositif m_Device;


    public:
        void update();

        void initializeDevice(HWND windowVisuHandle);


    public:
        void createCamera(const CameraParameters& cameraParameters);
        void setMainCameraMatViewManually(const DirectX::XMVECTOR& eyePosition, const DirectX::XMVECTOR& focusDirection, const DirectX::XMVECTOR& upDirection);


    public:
        DECLARE_SIMPLE_GET_REFERENCE_ACCESSOR(DXDispositif, Device);
    };
}

