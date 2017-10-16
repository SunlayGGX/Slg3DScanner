#pragma once

namespace Slg3DScanner
{
    enum class LoggerLevel : unsigned char
    {
        LEVEL_DEBUG,
        LEVEL_COMMENT,
        LEVEL_WARNING,
        LEVEL_ERROR,
        LEVEL_FATAL,

        LEVEL_COUNT
    };

    enum class LoggerType : unsigned char
    {
        CONSOLE,
        VISUAL_STUDIO_OUTPUT,
        FILE,

        TYPE_COUNT
    };
}

