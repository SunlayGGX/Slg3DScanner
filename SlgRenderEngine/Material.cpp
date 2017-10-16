#include "Material.h"

#include "UniqueDX.h"
#include "DirectXUtilitary.h"

#include "ShaderManager.h"
#include "RenderEngineManager.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "ShaderConstantBuffer.h"

using namespace Slg3DScanner;


Material::Material(const MaterialInitializer& initializer) :
    m_VertexShader{ std::make_shared<VertexShader>(initializer.m_VSFileName, initializer.m_VSFuncName) },
    m_PixelShader{ std::make_shared<PixelShader>(initializer.m_PSFileName, initializer.m_PSFuncName) },
    m_ConstantBuffer{ nullptr }
{
    this->internalInitializeShaders(initializer.m_Desc, initializer.m_ElemCount);
}

Material::Material(MaterialInitializer&& initializer) :
    m_VertexShader{ std::make_shared<VertexShader>(std::move(initializer.m_VSFileName), std::move(initializer.m_VSFuncName)) },
    m_PixelShader{ std::make_shared<PixelShader>(std::move(initializer.m_PSFileName), std::move(initializer.m_PSFuncName)) },
    m_ConstantBuffer{ nullptr }
{
    this->internalInitializeShaders(initializer.m_Desc, initializer.m_ElemCount);
}

Material::~Material()
{
    DXRelease(m_ConstantBuffer);
}

void Material::internalInitializeShaders(const D3D11_INPUT_ELEMENT_DESC* desc, UINT elemCount)
{
    ShaderManager& shaderMgr = ShaderManager::instance();

    shaderMgr.compile(m_VertexShader->shared_from_this(), desc, elemCount);

    D3D11_BUFFER_DESC bufferDesc;
    DXZeroMemory(bufferDesc);

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(Slg3DScanner::ForwardShadingConstantBufferParameter);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    UniqueDX<ID3D11Buffer> autoDestroyer{ m_ConstantBuffer };

    RenderEngineManager::instance().getDevice().getD3DDevice()->CreateBuffer(&bufferDesc, nullptr, &m_ConstantBuffer);

    shaderMgr.compile(m_PixelShader->shared_from_this(), desc, elemCount);

    autoDestroyer.release();
}

void Material::prepareDraw(ID3D11DeviceContext* immediateContext, const ForwardShadingConstantBufferParameter& renderCBufferParameter)
{
    immediateContext->VSSetShader(m_VertexShader->getVertexShaderRessource(), nullptr, 0);

    immediateContext->GSSetShader(nullptr, nullptr, 0);

    immediateContext->IASetInputLayout(m_VertexShader->getInputLayout());

    immediateContext->PSSetShader(m_PixelShader->getPixelShaderRessource(), nullptr, 0);

    immediateContext->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &renderCBufferParameter, 0, 0);

    immediateContext->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);
    immediateContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
}
