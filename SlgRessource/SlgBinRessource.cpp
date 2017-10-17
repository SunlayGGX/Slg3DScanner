#include "SlgRessourcePCH.h"
#include "SlgBinRessource.h"

#include <fstream>
#include <experimental/filesystem>

using namespace Slg3DScanner;

SlgBinRessource::SlgBinRessource()
{

}

SlgBinRessource::SlgBinRessource(const SlgBinRessource& other) = default;

SlgBinRessource::SlgBinRessource(SlgBinRessource&& other) :
    m_dataBuffer{ std::move(other.m_dataBuffer) }
{

}

SlgBinRessource::~SlgBinRessource() = default;

const std::vector<char>& SlgBinRessource::getData() const noexcept
{
    return m_dataBuffer;
}

bool SlgBinRessource::isLoaded() const noexcept
{
    return !m_dataBuffer.empty();
}

bool SlgBinRessource::load(const std::string& path)
{
    if(!path.empty())
    {
        std::ifstream file{ path.c_str(), std::ios::binary };
        if(file)
        {
            m_dataBuffer.clear();
            m_dataBuffer.reserve(std::experimental::filesystem::file_size(path));

            std::copy(
                std::istreambuf_iterator<char>{file}, 
                std::istreambuf_iterator<char>{}, 
                std::back_inserter(m_dataBuffer)
            );

            return !m_dataBuffer.empty();
        }
    }

    return false;
}

bool SlgBinRessource::write(const std::string& path)
{
    if(!path.empty())
    {
        std::copy(
            std::begin(m_dataBuffer), 
            std::end(m_dataBuffer), 
            std::ostreambuf_iterator<char>{std::ofstream{ path , std::ios::binary }}
        );

        return true;
    }

    return false;
}

void SlgBinRessource::freeRessource()
{
    m_dataBuffer = std::vector<char>{};
}

void SlgBinRessource::swap(SlgBinRessource& other) noexcept
{
    std::swap(this->m_dataBuffer, other.m_dataBuffer);
}

SlgBinRessource& SlgBinRessource::operator=(const SlgBinRessource& other) = default;

SlgBinRessource& SlgBinRessource::operator=(SlgBinRessource&& other)
{
    this->swap(SlgBinRessource{ std::move(other) });
    return *this;
}

bool SlgBinRessource::operator==(const SlgBinRessource& other) const
{
    return this->m_dataBuffer == other.m_dataBuffer;
}
