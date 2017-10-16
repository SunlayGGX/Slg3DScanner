#include "VertexShader.h"

#include "ShaderDebugDefineMacro.h"

#include "DirectXUtilitary.h"
#include "UniqueDX.h"

#include "RenderEngineManager.h"

#include <d3dcompiler.h>


using namespace Slg3DScanner;


VertexShader::VertexShader(const std::wstring& name, const std::string& funcName) :
    IShader{ name, funcName },
    m_VertexShaderRessource{ nullptr } 
{
}

VertexShader::VertexShader(std::wstring&& name, std::string&& funcName) :
    IShader{ std::move(name), std::move(funcName) },
    m_VertexShaderRessource{ nullptr }
{

}


VertexShader::~VertexShader()
{
    DXRelease(m_VertexShaderRessource);
}

void VertexShader::compile(CompileDelegate compileDelegate, const D3D11_INPUT_ELEMENT_DESC* desc, UINT elemCount)
{
    UniqueDX<ID3DBlob> vsBlob;
    
#ifndef SHADER_COMPILE_DEBUG_DETAIL
    DXTry(
#else
    UniqueDX<ID3DBlob> detailErrorBlob;

    auto result = 
#endif
        compileDelegate(
            this->getName().c_str(),
            m_Macro,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            this->getFuncName().c_str(),
            "vs_4_1",
#ifdef _DEBUG
            D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS
#else
            D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_SKIP_VALIDATION
#endif
            ,
            0,
            &vsBlob.get(),
#ifdef SHADER_COMPILE_DEBUG_DETAIL
            &detailErrorBlob.get()
        );
#else
        nullptr
    ));
#endif
    
#ifdef SHADER_COMPILE_DEBUG_DETAIL
    if(!FAILED(result))
    {
        LoggerCommand::logOnConsole("Shader compilation OK", Slg3DScanner::LoggerLevel::LEVEL_DEBUG);
    }
    else
    {
        LoggerCommand::logOnConsole(std::string{ static_cast<LPCSTR>(detailErrorBlob.get()->GetBufferPointer()) }, Slg3DScanner::LoggerLevel::LEVEL_ERROR);
        return;
    }
#endif

    ID3D11Device* dxDevice = RenderEngineManager::instance().getDevice().getD3DDevice();

    LPVOID bufferPointer = vsBlob->GetBufferPointer();
    SIZE_T bufferSize = vsBlob->GetBufferSize();

    DXTry(
        dxDevice->CreateVertexShader(
            bufferPointer,
            bufferSize,
            nullptr,
            &m_VertexShaderRessource
        )
    );

    DXTry(
        dxDevice->CreateInputLayout(
            desc,
            elemCount,
            bufferPointer,
            bufferSize,
            &m_InputLayout
        )
    );
}
