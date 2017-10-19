#include "SlgRessourcePCH.h"
#include "SlgBinRessource.h"

#include <fstream>
#include <experimental/filesystem>

using namespace Slg3DScanner;

SlgBinRessource::SlgBinRessource(const SlgBinRessource& other)
{
    std::unique_ptr<char[]> responsiblePtr = std::make_unique<char[]>(other.size());
    memcpy(responsiblePtr.get(), other.data(), other.size());
    m_dataBuffer = responsiblePtr.release();
    m_size = other.size();
}

SlgBinRessource::SlgBinRessource(SlgBinRessource&& other) noexcept :
    m_dataBuffer{ other.m_dataBuffer },
    m_size{ other.m_size }
{
    other.m_dataBuffer = nullptr;
}

SlgBinRessource::~SlgBinRessource()
{
    delete[] m_dataBuffer;
}

char* SlgBinRessource::data() const noexcept
{
    return m_dataBuffer;
}

std::size_t SlgBinRessource::size() const noexcept
{
    return m_size;
}

bool SlgBinRessource::isLoaded() const noexcept
{
    return m_dataBuffer != nullptr;
}

bool SlgBinRessource::load(const std::string& path)
{
    this->freeRessource();

    if(!path.empty())
    {
        std::ifstream file{ path.c_str(), std::ios::binary };
        if(file)
        {
            this->internalAllocate(std::experimental::filesystem::file_size(path));

            std::copy(
                std::istreambuf_iterator<char>{ file },
                std::istreambuf_iterator<char>{},
                m_dataBuffer
            );

            return this->isLoaded();
        }
    }

    return false;
}

bool SlgBinRessource::write(const std::string& path)
{
    if(!path.empty() && this->isLoaded())
    {
        std::copy(
            m_dataBuffer, 
            m_dataBuffer + m_size, 
            std::ostreambuf_iterator<char>{std::ofstream{ path , std::ios::binary }}
        );

        return true;
    }

    return false;
}

void SlgBinRessource::internalAllocate(const std::size_t newSize)
{
    m_dataBuffer = new char[newSize];
    m_size = newSize;
}

void SlgBinRessource::freeRessource() noexcept
{
    delete[] m_dataBuffer;
    m_dataBuffer = nullptr;
    m_size = 0;
}

void SlgBinRessource::swap(SlgBinRessource& other) noexcept
{
    std::swap(this->m_dataBuffer, other.m_dataBuffer);
    std::swap(this->m_size, other.m_size);
}

SlgBinRessource& SlgBinRessource::operator=(const SlgBinRessource& other)
{
    this->swap(SlgBinRessource{ other });
    return *this;
}

SlgBinRessource& SlgBinRessource::operator=(SlgBinRessource&& other)
{
    this->swap(SlgBinRessource{ std::move(other) });
    return *this;
}
