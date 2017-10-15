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

        //IS_FULL_SCREEN = false
    };

    enum class WantedResolution
    {
        HEIGHT = 600,
        WIDTH = 1800
    };
}
