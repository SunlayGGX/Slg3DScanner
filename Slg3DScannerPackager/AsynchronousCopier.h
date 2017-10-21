#pragma once

namespace Slg3DScanner
{
    namespace Tools
    {
        class IgnoreParser;
        class AsynchronousCopier
        {
        public:
            using BindedValue = std::pair<std::experimental::filesystem::path, std::experimental::filesystem::path>;
            using BindingPathArray = std::vector<BindedValue>;

        public:
            class WaitForCompletion {};
            class IndependentRun {};

        private:
            static std::mutex s_creationMutex;

        private:
            BindingPathArray m_sourceAndDestinationFolderBindPath;
            BindingPathArray m_sourceAndDestinationFileBindPath;

            mutable std::mutex m_mutex;

        public:
            AsynchronousCopier();
            AsynchronousCopier(AsynchronousCopier&) = delete;
            AsynchronousCopier& operator=(AsynchronousCopier&) = delete;
            ~AsynchronousCopier();

            void bindSourceToDestinationPath(std::experimental::filesystem::path sourcePath, std::experimental::filesystem::path destinationPath, bool fileUniCopy);

        private:
            void start(const IgnoreParser& ignoreParser, AsynchronousCopier::WaitForCompletion);
            void start(const IgnoreParser& ignoreParser, AsynchronousCopier::IndependentRun);

        private:
            static void internalCreateDestinationDirIfDoesntExists(const std::experimental::filesystem::path& desdestinationPathtination, const bool isFile);

        public:
            static void copyRelevantDirectoryRecursive(
                const std::experimental::filesystem::path& destinationPath,
                std::experimental::filesystem::directory_iterator directoryIterator, 
                const std::vector<std::experimental::filesystem::path>& ignore
            );

            static void copyFile(const std::experimental::filesystem::path& sourcePath, const std::experimental::filesystem::path& destinationPath, const std::vector<std::experimental::filesystem::path>& ignore);

        public:
            template<class RunOption>
            std::enable_if_t<
                std::is_same<RunOption, AsynchronousCopier::WaitForCompletion>::value || std::is_same<RunOption, AsynchronousCopier::IndependentRun>::value, 
                void
            > start(const IgnoreParser& ignoreParser)
            {
                this->start(ignoreParser, RunOption{});
            }
        };
    }
}
