#pragma once

#include "NamedObject.h"
#include "CRTPHierarchy.h"

namespace Slg3DScanner
{
    class SlgBinRessource
    {
    private:
        char* m_dataBuffer = nullptr;
        std::size_t m_size = 0;


    public:
        constexpr SlgBinRessource() noexcept = default;
        SlgBinRessource(const SlgBinRessource& other);
        SlgBinRessource(SlgBinRessource&& other) noexcept;
        ~SlgBinRessource();


    public:
        char* data() const noexcept;
        std::size_t size() const noexcept;

        bool isLoaded() const noexcept;

        void swap(SlgBinRessource& other) noexcept;

        void freeRessource() noexcept;

        bool load(const std::string& path);
        bool write(const std::string& path);


    private:
        void internalAllocate(const std::size_t newSize);


    public:
        SlgBinRessource& operator=(SlgBinRessource&& other);
        SlgBinRessource& operator=(const SlgBinRessource& other);


    public:
        template<class Type>
        Type* getAs(std::size_t& outSizeAsType, const std::size_t byteOffset) const
        {
            outSizeAsType = (m_size - byteOffset) / sizeof(Type);
            return reinterpret_cast<Type*>(m_dataBuffer + byteOffset);
        }

        template<class Type>
        Type* releaseAs(std::size_t& outSizeAsType)
        {
            Type* result = this->getAs<Type>(outSizeAsType, 0);
            
            m_dataBuffer = nullptr;
            m_size = 0;

            return result;
        }

        /*Take the ownership of data. data becomes nullptr afterwards.*/
        template<class Type>
        void setData(Type*& data, std::size_t size)
        {
            delete[] m_dataBuffer;
            m_dataBuffer = reinterpret_cast<char*>(data);
            data = nullptr;
            m_size = size * sizeof(Type);
        }

        template<>
        void setData<char>(char*& data, std::size_t size)
        {
            delete[] m_dataBuffer;
            m_dataBuffer = data;
            data = nullptr;
            m_size = size;
        }
    };
}
