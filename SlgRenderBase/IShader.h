#pragma once

#include "UtilsMacros.h"


namespace Slg3DScanner
{
    class IShader : public std::enable_shared_from_this<IShader>
    {
    public:
        using CompileDelegate = HRESULT(WINAPI*)(_In_ LPCWSTR pFileName,
            _In_reads_opt_(_Inexpressible_(pDefines->Name != NULL)) CONST D3D_SHADER_MACRO* pDefines,
            _In_opt_ ID3DInclude* pInclude,
            _In_ LPCSTR pEntrypoint,
            _In_ LPCSTR pTarget,
            _In_ UINT Flags1,
            _In_ UINT Flags2,
            _Out_ ID3DBlob** ppCode,
            _Out_opt_ ID3DBlob** ppErrorMsgs);


    private:
        std::wstring m_Name;
        std::string m_FuncName;


    protected:
        D3D10_SHADER_MACRO* m_Macro;

        ID3D11InputLayout* m_InputLayout;


    public:
        IShader(const std::wstring& name, const std::string& funcName);
        IShader(std::wstring&& name, std::string&& funcName);
        virtual ~IShader();


        DECLARE_SIMPLE_GET_REFERENCE_ACCESSOR(std::wstring, Name);
        DECLARE_SIMPLE_GET_REFERENCE_ACCESSOR(std::string, FuncName);
        DECLARE_SIMPLE_GET_ACCESSOR(D3D10_SHADER_MACRO*, Macro);
        DECLARE_SIMPLE_GET_ACCESSOR(ID3D11InputLayout*, InputLayout);

        virtual void compile(CompileDelegate compileDelegate, const D3D11_INPUT_ELEMENT_DESC* desc, UINT elemCount) = 0;
    };
}



