#pragma once


#include "SlgSingleton.h"
#include "FacetteAttribute.h"
#include "IShader.h"

namespace Slg3DScanner
{
    DECLARE_SLG_SINGLETON_ALLOCATOR_HELPER_FOR(ShaderManager)

    class ShaderManager : private SlgSingleton<ShaderManager>
    {
    private:
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(ShaderManager);


    private:
        IShader::CompileDelegate m_compileDelegate;


    public:
        /*
        Dynamic array input layout version
        */
        template<class ShaderType>
        void compile(const std::shared_ptr<ShaderType>& shader, const D3D11_INPUT_ELEMENT_DESC* desc, UINT elemCount)
        {
            shader->compile(m_compileDelegate, desc, elemCount);
        }
    };
}


