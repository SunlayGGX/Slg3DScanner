#pragma once

#pragma warning(disable: 4251)


namespace Slg3DScanner
{
    class Logger
    {
    private:
        std::unique_ptr<class LoggerPImpl> m_logger;
        bool m_active;
        mutable std::mutex m_mutex;


    public:
        Logger();
        ~Logger();


    public:
        Logger& operator<<(const std::string& toFlush);
        Logger& operator<<(float value);
        Logger& operator<<(int value);
        Logger& operator<<(const char* toFlush);

        void initializeAsConsoleLogger();
        void initializeAsVisualOutputLogger();
        void initializeAsFileLogger(const std::string& fileName);

        void setActive(bool active);
    };
}


#define CONSOLE_LOGGER Console
#define VISUAL_STUDIO_OUTPUT_LOGGER VisualOutput
#define FILE_LOGGER File

#ifdef _DEBUG

#   define DECLARE_DEBUG_LOGGER(logger) Slg3DScanner::Logger logger

/*Mode : Use macros CONSOLE_LOGGER, VISUAL_STUDIO_OUTPUT_LOGGER or FILE_LOGGER*/
#   define INIT_DEBUG_LOGGER_AS(logger, mode, ...) (logger.CONCATS(initializeAs, mode, Logger)(__VA_ARGS__))


#   define SLGENGINE_LOG_DEBUG_ON_CUSTOM_LOGGER(logger, toFlushVal) logger << toFlushVal
#else

#   define SLGENGINE_DECLARE_DEBUG_LOGGER(logger)
#   define SLGENGINE_INIT_DEBUG_LOGGER_AS(logger, mode, ...)
#   define SLGENGINE_LOG_DEBUG_ON_CUSTOM_LOGGER(logger, toFlushVal)

#endif

#define LOG_DEBUG_BOOL(logger, boolExpression)      LOG_DEBUG(logger, (boolExpression ? "true" : "false"))
#define LOG_DEBUG_BITBOOL(logger, boolExpression)   LOG_DEBUG(logger, (boolExpression ? "1" : "0"))
#define LOG_DEBUG_NULLITY(logger, ptr)              LOG_DEBUG(logger, (ptr != nullptr ? "Not Null" : "Null"))

