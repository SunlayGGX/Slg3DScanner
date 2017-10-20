#include "IShader.h"

#include "DirectXUtilitary.h"

using namespace Slg3DScanner;


IShader::IShader(const std::wstring& name, const std::string& funcName) :
    m_Name{ name },
    m_FuncName{ funcName },
    m_Macro{ nullptr },
    m_InputLayout{ nullptr }
{
}

IShader::IShader(std::wstring&& name, std::string&& funcName) :
    m_Name{ std::move(name) },
    m_FuncName{ std::move(funcName) },
    m_Macro{ nullptr },
    m_InputLayout{ nullptr }
{
}

IShader::~IShader()
{
    DXRelease(m_InputLayout);
}
