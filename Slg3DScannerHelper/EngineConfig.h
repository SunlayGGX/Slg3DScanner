#pragma once


namespace Slg3DScanner
{
    enum class WindowsMode : BOOL
    {
        FULL_SCREEN = FALSE,
        WINDOWED = TRUE
    };

    enum class Constants
    {
        FPS = 60,

        WINDOWS_THREAD_UPDATE_RATE_IN_MILLISECONDS = 250,

        WINDOWS_CONFIG_MODALITY = static_cast<BOOL>(Slg3DScanner::WindowsMode::WINDOWED)
    };

    enum class WantedResolution
    {
        HEIGHT = 600,
        WIDTH = 1800
    };

    static constexpr const float THRESHOLD_ABERRANT_TRIANGLE = 0.000025f;
}
