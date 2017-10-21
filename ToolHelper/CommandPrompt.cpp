#include "ToolHelperPCH.h"
#include "CommandPrompt.h"


using namespace Slg3DScanner::Tools;

CommandPrompt::CommandPrompt()
{
}

CommandPrompt::~CommandPrompt()
{
}

void CommandPrompt::addCommand(std::string command, int expectedReturnCode)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    m_command.emplace_back(std::move(command), expectedReturnCode);
}

void CommandPrompt::addCommand(const char* command, int expectedReturnCode)
{
    this->addCommand(std::string{ command }, expectedReturnCode);
}

CommandPrompt::CommandResult CommandPrompt::executeCommand()
{
    std::vector<CommandPrompt::CommandType> commandTmp;
    {
        std::lock_guard<std::mutex> autoLocker{ m_mutex };
        std::size_t oldSizeValue = m_command.size();
        std::swap(commandTmp, m_command);
        m_command.reserve(oldSizeValue);
    }

    CommandPromptInfos resultInfos{ 0,0 };

    std::size_t endIter = commandTmp.size();
    for(; resultInfos.first != endIter; ++resultInfos.first)
    {
        auto& currentCommandToExecute = commandTmp[resultInfos.first];

        resultInfos.second = ::system(currentCommandToExecute.first.c_str());
        if(resultInfos.second != currentCommandToExecute.second)
        {
            return CommandPrompt::CommandResult{ std::move(resultInfos), std::move(currentCommandToExecute) };
        }
    }

    return CommandPrompt::CommandResult{ std::move(resultInfos), CommandPrompt::CommandType{ "", 0 } };
}

void CommandPrompt::resetCommand()
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    m_command.clear();
}
