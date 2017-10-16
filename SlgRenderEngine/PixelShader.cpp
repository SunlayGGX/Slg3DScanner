#include "PixelShader.h"

#include "ShaderDebugDefineMacro.h"

#include "RenderEngineManager.h"
#include "UniqueDX.h"

#include "DirectXUtilitary.h"

#include <d3dcompiler.h>


using namespace Slg3DScanner;


PixelShader::PixelShader(const std::wstring& name, const std::string& funcName) :
    IShader{ name, funcName },
    m_PixelShaderRessource{ nullptr }
{

}

PixelShader::PixelShader(std::wstring&& name, std::string&& funcName) :
    IShader{ std::move(name), std::move(funcName) },
    m_PixelShaderRessource{ nullptr }
{
}

PixelShader::~PixelShader()
{
    DXRelease(m_PixelShaderRessource);
}

void PixelShader::compile(CompileDelegate compileDelegate, const D3D11_INPUT_ELEMENT_DESC* desc, UINT elemCount)
{
    // Compilation et chargement du pixel shader
    UniqueDX<ID3DBlob> pixelShaderBlob;

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
            "ps_4_1",
#ifdef _DEBUG
            D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS
#else
            D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_SKIP_VALIDATION
#endif
            ,
            0,
            &pixelShaderBlob.get(),
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

    DXTry(RenderEngineManager::instance().getDevice().getD3DDevice()->CreatePixelShader(
        pixelShaderBlob->GetBufferPointer(),
        pixelShaderBlob->GetBufferSize(),
        nullptr,
        &m_PixelShaderRessource)
    );
}
