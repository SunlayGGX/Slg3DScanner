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
        virtual void initialize() override;
        virtual void destroy() override;

        void update();

        void initializeDevice(HWND windowVisuHandle);


    public:
        void createCamera(const CameraParameters& cameraParameters);


    public:
        DECLARE_SIMPLE_GET_REFERENCE_ACCESSOR(DXDispositif, Device);
    };
}
