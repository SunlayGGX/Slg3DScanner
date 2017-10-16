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


    class Material
    {
    private:
        std::shared_ptr<class VertexShader> m_VertexShader;
        std::shared_ptr<class PixelShader> m_PixelShader;

        ID3D11Buffer* m_ConstantBuffer;


    public:
        Material(const MaterialInitializer& initializer);
        Material(MaterialInitializer&& initializer);
        ~Material();

        void prepareDraw(ID3D11DeviceContext* immediateContext, const ForwardShadingConstantBufferParameter& renderCBufferParameter);


    private:
        void internalInitializeShaders(const D3D11_INPUT_ELEMENT_DESC* desc, UINT elemCount);
    };
}

