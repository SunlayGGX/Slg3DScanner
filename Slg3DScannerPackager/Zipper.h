#pragma once

namespace Slg3DScanner
{
    namespace Tools
    {
        class Zipper
        {
        private:
            Zipper() = delete;
            Zipper(Zipper&) = delete;
            Zipper& operator=(Zipper&) = delete;
            ~Zipper() = delete;

        public:
            // Need Powershell 3.0 and .Net framework 4 or higher to work correctly
            static bool zipFolder(const std::experimental::filesystem::path& structurePathToZip, const std::experimental::filesystem::path& structurePathToFlushZip);
        };
    }
}
