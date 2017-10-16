#pragma once

#include "RenderConfig.h"

namespace Slg3DScanner
{
    struct CameraParameters
    {
    public:
        float fovYAngle = DefaultCameraParameter::DEFAULT_FOV;
        float aspectRatio = DefaultCameraParameter::DEFAULT_ASPECT_RATIO;
        float nearPlane = DefaultCameraParameter::DEFAULT_NEAR_PLANE;
        float farPlane = DefaultCameraParameter::DEFAULT_FAR_PLANE;


    public:
        constexpr CameraParameters() noexcept = default;
    };
}
