#pragma once

#include "LogMessage.h"
#include "LoggerEnum.h"

#include "Logger.h"

namespace Slg3DScanner
{
    template<class SingletonType> class SlgSingletonAllocatorHelper;

    class LoggerEngine
    {
    private:
        friend class SlgSingletonAllocatorHelper<LoggerEngine>;


    public:
        enum
        {
            REFRESH_SLEEP_IN_MILLISECONDS = 400,
            DATE_TIME_SIZE_FORMAT = 32
        };


    private:
        using LogMessageArray = std::vector<LogMessage>;


    private:
        static LoggerEngine* s_instance;

        LogMessageArray m_logMessageBuffer1;
        LogMessageArray m_logMessageBuffer2;
        
        LogMessageArray* m_frontBuffer;
        LogMessageArray* m_backBuffer;

        mutable std::mutex m_loggerMutex;

        std::chrono::time_point<std::chrono::steady_clock> m_loggingTimeBegin;
        char m_dateTime[DATE_TIME_SIZE_FORMAT];

        Logger m_logger[static_cast<std::size_t>(LoggerType::TYPE_COUNT)];

        std::atomic<LoggerLevel> m_logLevelFilter;

        std::atomic<bool> m_run;


    private:
        LoggerEngine();
        LoggerEngine(const LoggerEngine&) = delete;
        ~LoggerEngine();

        LoggerEngine& operator=(LoggerEngine& other) = delete;


    public:        
        void initialize();
        void destroy();

        void logOnFile(std::string message, LoggerLevel level);
        void logOnVSOutput(std::string message, LoggerLevel level);
        void logOnConsoleOutput(std::string message, LoggerLevel level);

        void setLogLevelFilter(LoggerLevel level);
        LoggerLevel getLogLevelFilter() const;

        std::mutex& retrieveLogMutex() const;


    private:
        void update();

        std::string parseLogMessageTime(const LogMessage& logMessage) const;

        void internalSwapBuffer();


    public:
        static void bindToExistant(LoggerEngine* instance);


    public:
        inline static LoggerEngine& instance()
        {
            return *s_instance;
        }
    };
}
