#pragma once

#include "IShader.h"
#include "FacetteAttribute.h"


namespace Slg3DScanner
{
    class VertexShader : 
        virtual public IShader,
        virtual public FacetteAttribute<VertexShader, IShader>
    {
    private:
        ID3D11VertexShader* m_VertexShaderRessource;


    public:
        VertexShader(const std::wstring& name, const std::string& funcName);
        VertexShader(std::wstring&& name, std::string&& funcName);
        virtual ~VertexShader();


    public:
        DECLARE_SIMPLE_GET_ACCESSOR(ID3D11VertexShader*, VertexShaderRessource);


    public:
        void compile(CompileDelegate compileDelegate, const D3D11_INPUT_ELEMENT_DESC* desc, UINT elemCount) override;
    };
}

