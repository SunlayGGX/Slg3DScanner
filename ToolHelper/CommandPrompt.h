#pragma once

namespace Slg3DScanner
{
    namespace Tools
    {
        class CommandPrompt
        {
        public:
            using CommandType = std::pair<std::string, int>;
            using CommandPromptInfos = std::pair<std::size_t, int>;
            using CommandResult = std::pair<CommandPromptInfos, CommandType>;

        private:
            std::vector<CommandType> m_command;
            std::mutex m_mutex;

        public:
            CommandPrompt();
            ~CommandPrompt();

        public:
            void addCommand(std::string command, int expectedReturnCode);
            void addCommand(const char* command, int expectedReturnCode);

            CommandPrompt::CommandResult executeCommand();

            void resetCommand();
        };
    }
}
