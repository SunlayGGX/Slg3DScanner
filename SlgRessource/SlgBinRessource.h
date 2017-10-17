#pragma once

#include "NamedObject.h"
#include "CRTPHierarchy.h"

namespace Slg3DScanner
{
    class SlgBinRessource : 
        private CRTPNotEqual<SlgBinRessource>
    {
    private:
        std::vector<char> m_dataBuffer;


    public:
        SlgBinRessource();
        SlgBinRessource(const SlgBinRessource& other);
        SlgBinRessource(SlgBinRessource&& other);
        ~SlgBinRessource();


    public:
        const std::vector<char>& getData() const noexcept;

        bool isLoaded() const noexcept;

        void swap(SlgBinRessource& other) noexcept;

        void freeRessource();

        bool load(const std::string& path);
        bool write(const std::string& path);


    public:
        SlgBinRessource& operator=(SlgBinRessource&& other);
        SlgBinRessource& operator=(const SlgBinRessource& other);

        bool operator==(const SlgBinRessource& other) const;
    };
}
