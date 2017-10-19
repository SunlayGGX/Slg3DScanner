#pragma once

#include "SlgSingleton.h"

namespace Slg3DScanner
{
    class NameManager : private Slg3DScanner::SlgSingleton<NameManager>
    {
    private:
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(NameManager);


    public:
        using CountType = unsigned long long;
        using NameType = std::string;


    private:
        mutable std::mutex m_mutex;
        std::unordered_map<NameType, CountType> m_nameMap;


    public:
        NameType createUniqueName(const NameType& root);
        CountType getCurrentNameRootCount(const NameType& root);
        bool rootNameIsPresent(const NameType& root) const;
    };
}
