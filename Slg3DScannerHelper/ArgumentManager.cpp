#include "Slg3DScannerHelperPCH.h"
#include "ArgumentManager.h"

#include "EngineConfig.h"

#include <algorithm>

using namespace Slg3DScanner;

namespace Slg3DScanner
{
    namespace ArgumentDetails
    {
        struct ArgumentHandler
        {
        public:
            using ArgumentHandlersFunc = void(*)();

        public:
            int argc; 
            char** argv;

            std::vector<std::string> m_possibleArgumentSpecifier;
            std::vector<ArgumentHandlersFunc> m_argumentHandlerFunc;

        public:
            static ArgumentHandler s_handler;

        private:
            ArgumentHandler() :
                argc{ 0 },
                argv{ nullptr },
                m_possibleArgumentSpecifier{ ArgumentHandler::makePossibleArgumentSpecifiers() },
                m_argumentHandlerFunc{ ArgumentHandler::makeHandlersFuncArray() }
            {

            }

        public:
            ArgumentManager::ArgumentID checkArgument(const std::string& argument)
            {
                for(std::size_t allowedArgumentIter = 0; allowedArgumentIter != m_possibleArgumentSpecifier.size(); ++allowedArgumentIter)
                {
                    if (argument == m_possibleArgumentSpecifier[allowedArgumentIter])
                    {
                        return static_cast<ArgumentManager::ArgumentID>(allowedArgumentIter);
                    }
                }

                return ArgumentManager::ArgumentID::INVALID_ARGUMENT;
            }

        private:
            static std::vector<std::string> makePossibleArgumentSpecifiers()
            {
                return
                {
                    "--eliminationtrianglethreshold"
                };
            }

            static std::vector<ArgumentHandlersFunc> makeHandlersFuncArray()
            {
                return
                {
                    []()
                {
                }

                };
            }
        };

        ArgumentHandler ArgumentHandler::s_handler;
    }
}

void ArgumentManager::registerArguments(int argc, char* argv[])
{
    ArgumentDetails::ArgumentHandler::s_handler.argc = argc;
    ArgumentDetails::ArgumentHandler::s_handler.argv = argv;
}

ArgumentManager::Argument::Argument(ArgumentID id) :
    m_ID{ id }
{

}

ArgumentManager::ArgumentManager()
{
    const auto possibleArgumentSpecifierCount = ArgumentDetails::ArgumentHandler::s_handler.m_possibleArgumentSpecifier.size();
    m_arguments.reserve(possibleArgumentSpecifierCount);

    for(auto iter = 0; iter < possibleArgumentSpecifierCount; ++iter)
    {
        m_arguments.emplace_back(static_cast<ArgumentID>(iter));
    }

    this->initialize();
}

ArgumentManager::~ArgumentManager() = default;

void ArgumentManager::initialize()
{
    std::string currentArgument;

    for(auto iter = 1; iter < ArgumentDetails::ArgumentHandler::s_handler.argc; ++iter)
    {
        currentArgument = ArgumentDetails::ArgumentHandler::s_handler.argv[iter];
        std::transform(currentArgument.begin(), currentArgument.end(), currentArgument.begin(), ::tolower);

        ArgumentManager::ArgumentID handledId = ArgumentDetails::ArgumentHandler::s_handler.checkArgument(currentArgument);
        if (handledId != ArgumentManager::ArgumentID::INVALID_ARGUMENT)
        {
            m_arguments.emplace_back(handledId);
            Argument& currentArg = m_arguments.back();
        }
    }
}

void ArgumentManager::destroy()
{

}

int ArgumentManager::getArgumentCount() const noexcept
{
    return ArgumentDetails::ArgumentHandler::s_handler.argc;
}

char** ArgumentManager::getRawArguments() const noexcept
{
    return ArgumentDetails::ArgumentHandler::s_handler.argv;
}

std::string ArgumentManager::getAllowedArgumentPrettyPrinted() const
{
    std::string allowedArgument;
    allowedArgument.reserve(ArgumentDetails::ArgumentHandler::s_handler.m_possibleArgumentSpecifier.size() * 32);

    const auto endIter = ArgumentDetails::ArgumentHandler::s_handler.m_possibleArgumentSpecifier.end();
    for(auto iter = ArgumentDetails::ArgumentHandler::s_handler.m_possibleArgumentSpecifier.begin(); iter != endIter; ++iter)
    {
        allowedArgument += *iter + '\n';
    }

    return allowedArgument;
}
