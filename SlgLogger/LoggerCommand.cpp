#include "LoggerCommand.h"

#include "LoggerEngine.h"


void Slg3DScanner::LoggerCommand::initialize()
{
    LoggerEngine::instance().initialize();
}

void Slg3DScanner::LoggerCommand::destroy()
{
    LoggerEngine::instance().destroy();
}

void Slg3DScanner::LoggerCommand::logOnFile(std::string logMessage, Slg3DScanner::LoggerLevel level)
{
    LoggerEngine::instance().logOnFile(std::move(logMessage), level);
}

void Slg3DScanner::LoggerCommand::logOnConsole(std::string logMessage, Slg3DScanner::LoggerLevel level)
{
    LoggerEngine::instance().logOnConsoleOutput(std::move(logMessage), level);
}

void Slg3DScanner::LoggerCommand::logOnVisualStudioOutput(std::string logMessage, Slg3DScanner::LoggerLevel level)
{
    LoggerEngine::instance().logOnVSOutput(std::move(logMessage), level);
}
