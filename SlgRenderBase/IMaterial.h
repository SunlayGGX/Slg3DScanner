#pragma once

namespace Slg3DScanner
{
    struct ForwardShadingConstantBufferParameter;

    struct MaterialInitializer
    {
        /* Vertex Shader */
        std::wstring m_VSFileName;
        std::string m_VSFuncName;


        /* Pixel Shader */
        std::wstring m_PSFileName;
        std::string m_PSFuncName;

        const D3D11_INPUT_ELEMENT_DESC* m_Desc;
        UINT m_ElemCount;
    };


    class IMaterial
    {
    public:
        IMaterial();
        virtual ~IMaterial();

        virtual void prepareDraw(ID3D11DeviceContext* immediateContext, const ForwardShadingConstantBufferParameter& renderCBufferParameter) = 0;
    };
}
