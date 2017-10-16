#pragma once

#include "LoggerEnum.h"

namespace Slg3DScanner
{
    class LoggerCommand
    {
    private:
        ~LoggerCommand() = delete;

    public:
        static void initialize();
        static void destroy();

        static void logOnFile(std::string logMessage, Slg3DScanner::LoggerLevel level);
        static void logOnConsole(std::string logMessage, Slg3DScanner::LoggerLevel level);
        static void logOnVisualStudioOutput(std::string logMessage, Slg3DScanner::LoggerLevel level);
    };
}

#ifdef _DEBUG

#   define SLGENGINE_LOG_DEBUG(message) LoggerCommand::logOnConsole(message, Slg3DScanner::LoggerLevel::LEVEL_DEBUG)

#else

#   define SLGENGINE_LOG_DEBUG(message)

#endif