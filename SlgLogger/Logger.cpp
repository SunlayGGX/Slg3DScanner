#include "Logger.h"

#include <string>
#include <Windows.h>
#include <fstream>

namespace Slg3DScanner
{
    class LoggerPImpl 
    {
    public:
        LoggerPImpl() = default;
        virtual ~LoggerPImpl() = default;

        virtual LoggerPImpl& operator<<(const std::string& toFlush) = 0;
        virtual LoggerPImpl& operator<<(const char* toFlush) = 0;
        virtual LoggerPImpl& operator<<(float value) = 0;
        virtual LoggerPImpl& operator<<(int value) = 0;
    };

    class LoggerConsole final : public LoggerPImpl
    {
    private:
        HANDLE m_console;
        DWORD m_byteWritten;


    public:
        LoggerConsole() :
            m_byteWritten{ 0 }
        {
            FreeConsole();
            AllocConsole();

            m_console = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        ~LoggerConsole()
        {
            FreeConsole();
        }

        LoggerConsole& operator<<(const std::string& toFlush) override
        {
            DWORD count = static_cast<DWORD>(toFlush.size());

            if(count > 0)
            {
                for(const char* message = toFlush.c_str();; message += m_byteWritten)
                {
                    WriteConsoleA(m_console, message, count, &m_byteWritten, 0);

                    count -= m_byteWritten;

                    if(count == 0)
                    {
                        break;
                    }
                }
            }

            return (*this);
        }

        LoggerConsole& operator<<(const char* toFlush) override
        {
            return (*this) << std::string{ toFlush };
        }

        LoggerConsole& operator<<(float value) override
        {
            return (*this) << std::to_string(value);
        }

        LoggerConsole& operator<<(int value) override
        {
            return (*this) << std::to_string(value);
        }
    };

    class LoggerVisualOutput final : public LoggerPImpl
    {
    public:
        LoggerVisualOutput() = default;

        ~LoggerVisualOutput() = default;

        LoggerVisualOutput& operator<<(const std::string& toFlush) override
        {
            OutputDebugStringA(toFlush.c_str());

            return (*this);
        }

        LoggerVisualOutput& operator<<(const char* toFlush) override
        {
            OutputDebugStringA(toFlush);

            return (*this);
        }

        LoggerVisualOutput& operator<<(float value) override
        {
            return (*this) << std::to_string(value);
        }

        LoggerVisualOutput& operator<<(int value) override
        {
            return (*this) << std::to_string(value);
        }
    };

    class LoggerFile final : public LoggerPImpl
    {
    private:
        std::ofstream m_fileStream;


    public:
        LoggerFile(const std::string& filePath)
        {
            m_fileStream.open(filePath, std::ios_base::out);
        }

        ~LoggerFile()
        {
            m_fileStream.close();
        }

        LoggerFile& operator<<(const std::string& toFlush) override
        {
            m_fileStream.write(toFlush.c_str(), toFlush.size());

            return (*this);
        }

        LoggerFile& operator<<(const char* toFlush) override
        {
            return (*this) << std::string{ toFlush };
        }

        LoggerFile& operator<<(float value) override
        {
            return (*this) << std::to_string(value);
        }

        LoggerFile& operator<<(int value) override
        {
            return (*this) << std::to_string(value);
        }
    };
}



using namespace Slg3DScanner;

Logger::Logger() :
    m_active{ true }
{
}

Logger::~Logger()
{
}

Logger& Logger::operator<<(const std::string& toFlush)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };

    if(m_active)
    {
        (*m_logger) << toFlush;
    }

    return *this;
}

Logger& Logger::operator<<(const char* toFlush)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };

    if(m_active)
    {
        (*m_logger) << toFlush;
    }

    return *this;
}

Logger& Logger::operator<<(float value)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };

    if(m_active)
    {
        (*m_logger) << value;
    }

    return *this;
}

Logger& Logger::operator<<(int value)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };

    if(m_active)
    {
        (*m_logger) << value;
    }

    return *this;
}

void Logger::initializeAsConsoleLogger()
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };

    m_logger = std::make_unique<LoggerConsole>();
}

void Logger::initializeAsVisualOutputLogger()
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };

    m_logger = std::make_unique<LoggerVisualOutput>();
}

void Logger::initializeAsFileLogger(const std::string& fileName)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };

    m_logger = std::make_unique<LoggerFile>(fileName);
}

void Logger::setActive(bool active)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };

    m_active = active;
}
