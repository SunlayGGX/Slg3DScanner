#include "ShaderManager.h"

#include "DllManager.h"

using namespace Slg3DScanner;


ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::initialize()
{
    HMODULE d3dCompilerModule = DllManager::instance().loadExternDll("D3DCompiler_47.dll");

    m_compileDelegate = reinterpret_cast<IShader::CompileDelegate>(GetProcAddress(d3dCompilerModule, "D3DCompileFromFile"));

    if(m_compileDelegate == nullptr)
    {
        throw std::exception{ SLG_NORMALIZE_EXCEPTION_MESSAGE("Error when getting D3DCompileFromFileDelegate") };
    }
}

void ShaderManager::destroy()
{

}