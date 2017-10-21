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
            IgnoreParser();
            ~IgnoreParser();

        public:
            const std::vector<std::experimental::filesystem::path>& getAllExtensionToIgnore() const;

            void load(const std::string& extensionFile);

            void addToIgnoreList(const std::string& toAdd);
            void removeFromIgnoreList(const std::string& toRemove);

        private:
            bool isNotParseSeparator(char evaluatedCharacter) const;
            bool isNotSkipParseSeparator(char evaluatedCharacter) const;
        };
    }
}
