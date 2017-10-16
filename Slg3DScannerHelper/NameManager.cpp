#include "Slg3DScannerHelperPCH.h"
#include "NameManager.h"

using namespace Slg3DScanner;

NameManager::NameManager() = default;

NameManager::~NameManager() = default;

void NameManager::initialize()
{

}

void NameManager::destroy()
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    m_nameMap.clear();
}

NameManager::NameType NameManager::createUniqueName(const NameType& root)
{
    std::string countValue;
    
    {
        std::lock_guard<std::mutex> autoLocker{ m_mutex };
        countValue = std::to_string(m_nameMap[root]++);
    }

    return root + countValue;
}

NameManager::CountType NameManager::getCurrentNameRootCount(const NameType& root)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    return m_nameMap[root];
}

bool NameManager::rootNameIsPresent(const NameType& root) const
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    return m_nameMap.find(root) != m_nameMap.end();
}
