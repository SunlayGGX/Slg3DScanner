#pragma once

#include "IMaterial.h"


namespace Slg3DScanner
{
    struct ForwardShadingConstantBufferParameter;
    
    class Material : public IMaterial
    {
    private:
        std::shared_ptr<class VertexShader> m_vertexShader;
        std::shared_ptr<class PixelShader> m_pixelShader;

        ID3D11Buffer* m_constantBuffer;


    public:
        Material(const MaterialInitializer& initializer);
        Material(MaterialInitializer&& initializer);
        ~Material();

        void prepareDraw(ID3D11DeviceContext* immediateContext, const ForwardShadingConstantBufferParameter& renderCBufferParameter) override;


    private:
        void internalInitializeShaders(const D3D11_INPUT_ELEMENT_DESC* desc, UINT elemCount);
    };
}

