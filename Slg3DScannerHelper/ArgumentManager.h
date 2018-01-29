#pragma once

#include "SlgSingleton.h"

namespace Slg3DScanner
{
    class ArgumentManager : private Slg3DScanner::SlgSingleton<ArgumentManager>
    {
    private:
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(ArgumentManager);

    public:
        enum ArgumentID : short
        {
            ELIMINATION_TRIANGLE_THRESHOLD,

            INVALID_ARGUMENT,
        };

    private:
        struct Argument
        {
        public:
            ArgumentID m_ID;
            std::string m_value;

        public:
            Argument(ArgumentID id);
        };

    private:
        std::vector<Argument> m_arguments;

    public:
        int getArgumentCount() const noexcept;
        char** getRawArguments() const noexcept;

        std::string getAllowedArgumentPrettyPrinted() const;

    public:
        static void registerArguments(int argc, char* argv[]);
    };
}
