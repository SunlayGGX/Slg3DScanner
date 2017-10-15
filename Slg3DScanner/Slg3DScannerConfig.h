#pragma once

namespace Slg3DScanner
{
    enum class ApplicationResult : int
    {
        SUCCESS = 0,

        STANDARD_ERROR = 99,
        UNKNOWN_ERROR = 100,
    };

    enum class Config : int
    {
        APPLICATION_FPS = 60
    };
}
