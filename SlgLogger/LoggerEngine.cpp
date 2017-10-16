#include "LoggerPCH.h"
#include "LoggerEngine.h"

#include <algorithm>

#include <thread>
#include <ctime>

#include <experimental/filesystem>

using namespace Slg3DScanner;

LoggerEngine* LoggerEngine::s_instance = nullptr;

namespace Slg3DScanner
{
    const char* parseLogLevel(Slg3DScanner::LoggerLevel level)
    {
        static constexpr const char* levelLU[] =
        {
            " DEBUG : ",
            " COMMENT : ",
            " WARNING : ",
            " ERROR : ",
            " FATAL : "
        };

        return levelLU[static_cast<std::size_t>(level)];
    }
}


LoggerEngine::LoggerEngine() :
    m_backBuffer{ &m_logMessageBuffer2 },
    m_frontBuffer{ &m_logMessageBuffer1 }
{
}

LoggerEngine::~LoggerEngine()
{
    this->destroy();
    std::this_thread::sleep_for(std::chrono::milliseconds{ LoggerEngine::REFRESH_SLEEP_IN_MILLISECONDS });
}

void LoggerEngine::initialize()
{
    if(m_run)
    {
        return;
    }

    m_logger[static_cast<std::size_t>(Slg3DScanner::LoggerType::CONSOLE)].initializeAsConsoleLogger();
    m_logger[static_cast<std::size_t>(Slg3DScanner::LoggerType::VISUAL_STUDIO_OUTPUT)].initializeAsVisualOutputLogger();
    m_logger[static_cast<std::size_t>(Slg3DScanner::LoggerType::FILE)].initializeAsFileLogger(std::experimental::filesystem::current_path().string() + "/SLGENGINE_LOG.txt");

    m_run = true;

    m_loggingTimeBegin = std::chrono::high_resolution_clock::now();

    std::thread {
        [this]() {
            while(m_run)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds{ LoggerEngine::REFRESH_SLEEP_IN_MILLISECONDS });
                this->update();
            }
        }
    }.detach();
}

void LoggerEngine::destroy()
{
    m_run = false;
}

void LoggerEngine::bindToExistant(LoggerEngine* instance)
{
    if(s_instance != instance)
    {
        if(s_instance != nullptr && s_instance->m_run)
        {
            s_instance->destroy();
        }

        s_instance = instance;
    }
}

void LoggerEngine::update()
{
    time_t rawtime;

    time(&rawtime);

    std::strftime(m_dateTime, DATE_TIME_SIZE_FORMAT, "[%x %T]", localtime(&rawtime));

    std::for_each(
        m_backBuffer->begin(),
        m_backBuffer->end(),
        [this](const LogMessage& logMessage) {

            if(logMessage.getLevel() >= this->m_logLevelFilter)
            {
                m_logger[static_cast<std::size_t>(logMessage.getLoggerType())]
                    << this->m_dateTime
                    << this->parseLogMessageTime(logMessage)
                    << parseLogLevel(logMessage.getLevel())
                    << logMessage.getMessage()
                    << "\n";
            }
    });

    m_backBuffer->clear();

    this->internalSwapBuffer();
}

void LoggerEngine::logOnFile(std::string message, LoggerLevel level)
{
    if(m_run)
    {
        std::lock_guard<std::mutex> autoLocker{ this->retrieveLogMutex() };
        m_frontBuffer->emplace_back(std::move(message), level, Slg3DScanner::LoggerType::FILE);
    }
}

void LoggerEngine::logOnVSOutput(std::string message, LoggerLevel level)
{
    if(m_run)
    {
        std::lock_guard<std::mutex> autoLocker{ this->retrieveLogMutex() };
        m_frontBuffer->emplace_back(std::move(message), level, Slg3DScanner::LoggerType::VISUAL_STUDIO_OUTPUT);
    }
}

void LoggerEngine::logOnConsoleOutput(std::string message, LoggerLevel level)
{
    if(m_run)
    {
        std::lock_guard<std::mutex> autoLocker{ this->retrieveLogMutex() };
        m_frontBuffer->emplace_back(std::move(message), level, Slg3DScanner::LoggerType::CONSOLE);
    }
}

void LoggerEngine::setLogLevelFilter(LoggerLevel level)
{
    m_logLevelFilter = level;
}

LoggerLevel LoggerEngine::getLogLevelFilter() const
{
    return m_logLevelFilter;
}

std::mutex& LoggerEngine::retrieveLogMutex() const
{
    return m_loggerMutex;
}

void LoggerEngine::internalSwapBuffer()
{
    std::lock_guard<std::mutex> autoLocker{ this->retrieveLogMutex() };
    std::swap(m_frontBuffer, m_backBuffer);
}

std::string LoggerEngine::parseLogMessageTime(const LogMessage& logMessage) const
{
    std::chrono::nanoseconds nanosec = logMessage.getMessageTime() - this->m_loggingTimeBegin;

    auto nanoLong = nanosec.count();

    std::string messageTime = "[" + std::to_string(nanoLong / 1000000) + ":" + std::to_string(nanoLong / 1000) + "]";

    return messageTime;
}
