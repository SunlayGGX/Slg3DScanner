#pragma once

#include "RenderConfig.h"

namespace Slg3DScanner
{
    /*******************************************/

#pragma push_macro("SHADER_CONSTANT_BUFFER_VALIDATOR")

#define SHADER_CONSTANT_BUFFER_VALIDATOR(ShaderCBufferClass) \
    template<class ShaderCBufferParameter> \
    struct ShaderCBufferClass##ShaderConstantBufferValidator \
    { \
        static_assert((sizeof(ShaderCBufferParameter) % 16) == 0, "ERROR : "#ShaderCBufferClass" : Shader Constant Buffer class must have a size that is multiple of 16.\nBeware since aligment also plays a role into sizeof so sizeof something isn't necessarily the added sizeof of all members"); \
    }; \
    struct ShaderConstantBufferValidatorHelperInstanciatorFor##ShaderCBufferClass : private ShaderCBufferClass##ShaderConstantBufferValidator<ShaderCBufferClass>{}; 








    /*******************************************/

    struct PreInitializeCBufferParameterFromRendererSceneManager
    {
        DirectX::XMMATRIX m_TransposedMatViewProj;
        DirectX::XMVECTOR m_CameraPos;
        
        DirectX::XMVECTOR m_LightVect;
        DirectX::XMVECTOR m_LightAmbient = DefaultRenderCoefficientParameters::DEFAULT_LIGHT_AMBIENT_COEFFICIENT;
        DirectX::XMVECTOR m_LightDiffuse = DefaultRenderCoefficientParameters::DEFAULT_LIGHT_DIFFUSE_COEFFICIENT;
        DirectX::XMVECTOR m_LightSpecular = DefaultRenderCoefficientParameters::DEFAULT_LIGHT_SPECULAR_COEFFICIENT;
    };

    struct PreInitializeCBufferParameterFromMeshInstance
    {
        DirectX::XMMATRIX m_TransposedMatWorld = DefaultRenderCoefficientParameters::DEFAULT_MAT_WORLD_AT_CREATION;
        DirectX::XMVECTOR m_DiffuseAmbient = DefaultRenderCoefficientParameters::DEFAULT_MESH_AMBIENT_COEFFICIENT;
        DirectX::XMVECTOR m_DiffuseDiffuse = DefaultRenderCoefficientParameters::DEFAULT_MESH_DIFFUSE_COEFFICIENT;
        DirectX::XMVECTOR m_DiffuseSpecular = DefaultRenderCoefficientParameters::DEFAULT_MESH_SPECULAR_COEFFICIENT;
    };

    struct ForwardShadingConstantBufferParameter
    {
    public:
        DirectX::XMMATRIX m_TransposedMatWorldViewProj;
        DirectX::XMVECTOR m_CameraPos;

        DirectX::XMVECTOR m_LightVect;
        DirectX::XMVECTOR m_LightAmbient = DefaultRenderCoefficientParameters::DEFAULT_LIGHT_AMBIENT_COEFFICIENT;
        DirectX::XMVECTOR m_LightDiffuse = DefaultRenderCoefficientParameters::DEFAULT_LIGHT_DIFFUSE_COEFFICIENT;
        DirectX::XMVECTOR m_LightSpecular = DefaultRenderCoefficientParameters::DEFAULT_LIGHT_SPECULAR_COEFFICIENT;

        PreInitializeCBufferParameterFromMeshInstance m_paramsComingFromMeshInstance;
    };




    
    /******************************************/

    SHADER_CONSTANT_BUFFER_VALIDATOR(PreInitializeCBufferParameterFromRendererSceneManager);
    SHADER_CONSTANT_BUFFER_VALIDATOR(PreInitializeCBufferParameterFromMeshInstance);
    SHADER_CONSTANT_BUFFER_VALIDATOR(ForwardShadingConstantBufferParameter);





    /*****************************************/
    struct CopyConstantBufferHelper
    {
        template<class From, class To>
        static void copy(const From& from, To& to)
        {
            memcpy_s(static_cast<void*>(&to), sizeof(to), static_cast<const void*>(&from), sizeof(from));
        }
    };


    struct ConstantBufferParameterHelper
    {
        static void transfer(const PreInitializeCBufferParameterFromMeshInstance& inPreInit, ForwardShadingConstantBufferParameter& out)
        {
            CopyConstantBufferHelper::copy(inPreInit, out.m_paramsComingFromMeshInstance);
        }

        /*
        Does not solve conflict => thus m_MatWorldViewProj and PreInitializeCBufferParameterFromMeshInstance members aren't changed.
        */
        static void transfer(const PreInitializeCBufferParameterFromRendererSceneManager& inPreInit, ForwardShadingConstantBufferParameter& out)
        {
            CopyConstantBufferHelper::copy(inPreInit.m_CameraPos, out.m_CameraPos);
            CopyConstantBufferHelper::copy(inPreInit.m_LightVect, out.m_LightVect);
            CopyConstantBufferHelper::copy(inPreInit.m_LightAmbient, out.m_LightAmbient);
            CopyConstantBufferHelper::copy(inPreInit.m_LightDiffuse, out.m_LightDiffuse);
            CopyConstantBufferHelper::copy(inPreInit.m_LightSpecular, out.m_LightSpecular);
        }
    };



#undef SHADER_CONSTANT_BUFFER_VALIDATOR
#pragma pop_macro("SHADER_CONSTANT_BUFFER_VALIDATOR")
}
