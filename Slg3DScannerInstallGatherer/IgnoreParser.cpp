#include "Slg3DScannerInstallGathererPCH.h"
#include "IgnoreParser.h"

#include <fstream>

using namespace Slg3DScanner::Tools;

FORCEINLINE bool IgnoreParser::isNotParseSeparator(char evaluatedCharacter) const
{
    return !(
        evaluatedCharacter == ' '  ||
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

IgnoreParser::IgnoreParser(const std::string& extensionFile)
{
    using ReadDatatType = char;

    if(!std::experimental::filesystem::is_regular_file(extensionFile))
    {
        throw std::exception{ ("Cannot read the ignore file extension named " + extensionFile).c_str() };
    }

    std::size_t fileSize = std::experimental::filesystem::file_size(extensionFile);

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
        textIter = currentIter;

        //do not add doublons
        std::experimental::filesystem::path tempPath{ tmpExtension };
        if(std::find(m_LUNameToIgnore.begin(), m_LUNameToIgnore.end(), tempPath) == m_LUNameToIgnore.end())
        {
            m_LUNameToIgnore.emplace_back(std::move(tempPath));
        }

        tmpExtension.clear();
    }

    m_LUNameToIgnore.shrink_to_fit();
}

IgnoreParser::~IgnoreParser()
{
}

const std::vector<std::experimental::filesystem::path>& IgnoreParser::getAllExtensionToIgnore() const
{
    return m_LUNameToIgnore;
}
