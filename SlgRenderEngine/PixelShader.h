#pragma once

#include "IShader.h"
#include "FacetteAttribute.h"


namespace Slg3DScanner
{
    class PixelShader :
        virtual public IShader,
        virtual public FacetteAttribute<PixelShader, IShader>
    {
    private:
        ID3D11PixelShader* m_PixelShaderRessource;


    public:
        PixelShader(const std::wstring& name, const std::string& funcName);
        PixelShader(std::wstring&& name, std::string&& funcName);
        ~PixelShader();


    public:
        DECLARE_SIMPLE_GET_ACCESSOR(ID3D11PixelShader*, PixelShaderRessource);


        void compile(CompileDelegate compileDelegate, const D3D11_INPUT_ELEMENT_DESC* desc, UINT elemCount) override;
    };
}

