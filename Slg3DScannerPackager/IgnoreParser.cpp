#include "Slg3DScannerPackagerPCH.h"
#include "IgnoreParser.h"

#include <fstream>

using namespace Slg3DScanner::Tools;


IgnoreParser::IgnoreParser()
{
    
}

IgnoreParser::~IgnoreParser()
{
}

FORCEINLINE bool IgnoreParser::isNotParseSeparator(char evaluatedCharacter) const
{
    return !(
        evaluatedCharacter == ' ' ||
        evaluatedCharacter == '\t' ||
        evaluatedCharacter == '\n'
        );
}

FORCEINLINE bool IgnoreParser::isNotSkipParseSeparator(char evaluatedCharacter) const
{
    return !(
        evaluatedCharacter == '*'
        );
}

const std::vector<std::experimental::filesystem::path>& IgnoreParser::getAllExtensionToIgnore() const
{
    return m_LUNameToIgnore;
}

void IgnoreParser::addToIgnoreList(const std::string& toAdd)
{
    //do not add doublons
    std::experimental::filesystem::path tempPath{ toAdd };
    if(!toAdd.empty() && std::find(m_LUNameToIgnore.begin(), m_LUNameToIgnore.end(), tempPath) == m_LUNameToIgnore.end())
    {
        m_LUNameToIgnore.emplace_back(std::move(tempPath));
    }
}

void IgnoreParser::removeFromIgnoreList(const std::string& toRemove)
{
    auto found = std::find(m_LUNameToIgnore.begin(), m_LUNameToIgnore.end(), std::experimental::filesystem::path{ toRemove });
    if(found != m_LUNameToIgnore.end())
    {
        std::swap(*found, m_LUNameToIgnore.back());
        m_LUNameToIgnore.pop_back();
    }
}

void IgnoreParser::load(const std::string& extensionFile)
{
    using ReadDatatType = char;

    m_LUNameToIgnore.clear();

    if(!std::experimental::filesystem::is_regular_file(extensionFile))
    {
        throw std::exception{ ("Cannot read the ignore file extension named " + extensionFile).c_str() };
    }

    std::size_t fileSize = std::experimental::filesystem::file_size(extensionFile);

    if(fileSize == 0)
    {
        return;
    }

    std::string readText;
    readText.reserve(fileSize);

    std::copy(std::istreambuf_iterator<ReadDatatType>{ std::ifstream{ extensionFile } }, std::istreambuf_iterator<ReadDatatType>{}, std::back_inserter(readText));

    m_LUNameToIgnore.reserve(readText.size()); //better more than not enough. Shrink to fit afterwards.

    std::string tmpExtension;

    auto endTextIter = readText.end();
    for(auto textIter = readText.begin(); textIter != endTextIter; ++textIter)
    {
        auto currentIter = textIter;
        for(; currentIter != endTextIter; ++currentIter)
        {
            if(this->isNotSkipParseSeparator(*currentIter))
            {
                if(this->isNotParseSeparator(*currentIter))
                {
                    tmpExtension.push_back(*currentIter);
                }
                else
                {
                    break;
                }
            }
        }
        
        this->addToIgnoreList(tmpExtension);
        tmpExtension.clear();

        if(currentIter != endTextIter)
        {
            textIter = currentIter;
        }
        else
        {
            break;
        }
    }

    m_LUNameToIgnore.shrink_to_fit();
}
