#include "LogMessage.h"


using namespace Slg3DScanner;

LogMessage::LogMessage(std::string&& m_message, Slg3DScanner::LoggerLevel level, Slg3DScanner::LoggerType type) :
    m_message{ std::forward<std::string>(m_message) },
    m_level{ level },
    m_messageTime{ std::chrono::high_resolution_clock::now() },
    m_type{ type }
{

}

LogMessage::~LogMessage() = default;

LogMessage::LogMessageTimeType LogMessage::getMessageTime() const
{
    return m_messageTime;
}

const std::string& LogMessage::getMessage() const
{
    return m_message;
}

Slg3DScanner::LoggerLevel LogMessage::getLevel() const
{
    return m_level;
}

Slg3DScanner::LoggerType LogMessage::getLoggerType() const
{
    return m_type;
}
