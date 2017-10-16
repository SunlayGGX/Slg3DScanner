#pragma once

namespace Slg3DScanner
{
    class LoggerEngine;
    class LoggerBind
    {
    private:
        LoggerBind() = delete;
        LoggerBind(LoggerBind&) = delete;
        ~LoggerBind() = delete;
        LoggerBind& operator=(LoggerBind&) = delete;


    public:
        static void bindToExistant(LoggerEngine* existantInstance);
    };
}
