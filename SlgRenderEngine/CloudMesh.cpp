#include "SlgRenderEnginePCH.h"
#include "CloudMesh.h"

#include "SlgCloudReadWrite.h"
#include "DirectXUtilitary.h"

#include "RenderEngineManager.h"

#include "CloudVertex.h"

#include <experimental/filesystem>
#include <future>

using namespace Slg3DScanner;

CloudMesh::CloudMesh(const CloudMeshInitializer& cloudMeshInitializer) :
    NamedObject{ cloudMeshInitializer.name },
    Mesh{ cloudMeshInitializer },
    m_vertexCount{ 0 },
    m_initialized{ false },
    m_cloudTopology{ D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST }
{
    this->setCloudFile(cloudMeshInitializer.m_cloudFileName);
    this->readCloudFile();
}

CloudMesh::~CloudMesh() = default;

void CloudMesh::setCloudFile(const std::string &cloudFileName)
{
    std::experimental::filesystem::path filePath{ cloudFileName };
    if(filePath.has_extension())
    {
        this->internalSetCloudFile(cloudFileName);
    }
    else
    {
        this->internalSetCloudFile(cloudFileName + ".slgBinPos");
    }
}

void CloudMesh::internalSetCloudFile(std::string cloudFileName)
{
    if(std::experimental::filesystem::exists(cloudFileName))
    {
        m_cloudFileName = std::move(cloudFileName);
    }
    else
    {
        throw std::exception{ SLG_NORMALIZE_EXCEPTION_STRING_MESSAGE(cloudFileName + " doesn't exists") };
    }
}

void CloudMesh::draw(ID3D11DeviceContext* immediateContext, const PreInitializeCBufferParameterFromRendererSceneManager& preInitShadingCBuffer)
{
    if(m_initialized)
    {
        D3D11_PRIMITIVE_TOPOLOGY oldPrimitiveTopology;
        immediateContext->IAGetPrimitiveTopology(&oldPrimitiveTopology);
        immediateContext->IASetPrimitiveTopology(m_cloudTopology);

        ForwardShadingConstantBufferParameter renderCBufferParameter;

        ConstantBufferParameterHelper::transfer(preInitShadingCBuffer, renderCBufferParameter);

        {
            std::lock_guard<std::mutex> autoLocker{ m_mutex };

            const PreInitializeCBufferParameterFromMeshInstance& thisMeshParameters = this->getMeshParams();

            ConstantBufferParameterHelper::transfer(thisMeshParameters, renderCBufferParameter);

            /*Those matrix are transposed : transpose(A * B) == transpose(B) * transpose(A) */
            renderCBufferParameter.m_TransposedMatWorldViewProj = preInitShadingCBuffer.m_TransposedMatViewProj * thisMeshParameters.m_TransposedMatWorld;

            auto endIter = m_materialArray.end();
            for(auto iter = m_materialArray.begin(); iter != endIter; ++iter)
            {
                iter->prepareDraw(immediateContext, renderCBufferParameter);

                immediateContext->DrawIndexed(static_cast<UINT>(m_vertexCount), 0, 0);
            }
        }

        immediateContext->IASetPrimitiveTopology(oldPrimitiveTopology);
    }
}

void CloudMesh::readCloudFile()
{
    m_initialized = false;

    std::thread
    {
        [this]() {
        m_cloud = Slg3DScanner::readPointCloudfromFile(m_cloudFileName, m_vertexCount);
        if(m_vertexCount == 0)
        {
            throw std::exception{ SLG_NORMALIZE_EXCEPTION_MESSAGE("No point cloud created") };
        }

        this->internalSendDataToGraphicCard();

        m_initialized = true;
    } }.detach();
}

void CloudMesh::internalSendDataToGraphicCard()
{
    ID3D11Device* device = RenderEngineManager::instance().getDevice().getD3DDevice();

    D3D11_BUFFER_DESC bufferDesc;
    D3D11_SUBRESOURCE_DATA initData;

    DXZeroMemory(
        bufferDesc,
        initData
    );
    
    bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = static_cast<UINT>(m_vertexCount * sizeof(VertexType));
    bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    initData.pSysMem = m_cloud.get();

    DXTry(device->CreateBuffer(&bufferDesc, &initData, &m_vertexBuffer));

    std::unique_ptr<UINT[]> indexBuffer = std::make_unique<UINT[]>(m_vertexCount);
    for(unsigned int iter = 0; iter != m_vertexCount; ++iter)
    {
        indexBuffer[iter] = iter;
    }

    bufferDesc.ByteWidth = static_cast<UINT>(m_vertexCount * sizeof(UINT));
    bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;

    initData.pSysMem = indexBuffer.get();

    DXTry(device->CreateBuffer(&bufferDesc, &initData, &m_indexBuffer));

    this->setBuffers(RenderEngineManager::instance().getDevice().getImmediateContext(), sizeof(VertexType), 0);
}
