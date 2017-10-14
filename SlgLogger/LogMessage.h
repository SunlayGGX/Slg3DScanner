#pragma once

#include "LoggerEnum.h"

namespace Slg3DScanner
{
    class LogMessage
    {
    public:
        using LogMessageTimeType = std::chrono::time_point<std::chrono::steady_clock>;


    private:
        LogMessageTimeType m_messageTime;
        std::string m_message;
        Slg3DScanner::LoggerLevel m_level;
        Slg3DScanner::LoggerType m_type;


    public:
        LogMessage(std::string&& m_message, Slg3DScanner::LoggerLevel level, Slg3DScanner::LoggerType type);
        ~LogMessage();


    public:
        LogMessageTimeType getMessageTime() const;
        const std::string& getMessage() const;
        Slg3DScanner::LoggerLevel getLevel() const;
        Slg3DScanner::LoggerType getLoggerType() const;
    };
}
