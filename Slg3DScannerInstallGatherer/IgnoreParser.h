#pragma once

namespace Slg3DScanner
{
    namespace Tools
    {
        class IgnoreParser
        {
        private:
            std::vector<std::experimental::filesystem::path> m_LUNameToIgnore;

        public:
            IgnoreParser(const std::string& extensionFile);
            ~IgnoreParser();

        public:
            const std::vector<std::experimental::filesystem::path>& getAllExtensionToIgnore() const;

        private:
            bool isNotParseSeparator(char evaluatedCharacter) const;
            bool isNotSkipParseSeparator(char evaluatedCharacter) const;
        };
    }
}
