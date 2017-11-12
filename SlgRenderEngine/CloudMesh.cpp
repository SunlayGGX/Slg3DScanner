#include "SlgRenderEnginePCH.h"
#include "CloudMesh.h"

#include "SlgCloudReadWrite.h"
#include "DirectXUtilitary.h"

#include "RenderEngineManager.h"

#include "CloudVertex.h"
#include "CloudMerger.h"

#include <experimental/filesystem>
#include <future>

using namespace Slg3DScanner;

CloudMesh::CloudMesh(const CloudMeshInitializer& cloudMeshInitializer) :
    NamedObject{ cloudMeshInitializer.name },
    Mesh{ cloudMeshInitializer },
    m_initialized{ false },
    m_cloudTopology{ static_cast<D3D11_PRIMITIVE_TOPOLOGY>(CloudMesh::CURRENT_INDEX_TOPOLOGY) },
    m_version{ CloudMesh::Version::UNKNOWN }
{
    this->setCloudFile(cloudMeshInitializer.m_cloudFilesName);
    this->readCloudFile();
}

CloudMesh::~CloudMesh() = default;

void CloudMesh::setCloudFile(const std::vector<std::string>& cloudFileName)
{
    m_cloudFilesName.clear();
    m_cloudFilesName.reserve(cloudFileName.size());
    m_version = CloudMesh::Version::UNKNOWN;

    const auto endIter = cloudFileName.end();
    for(auto iter = cloudFileName.begin(); iter != endIter; ++iter)
    {
        std::experimental::filesystem::path filePath{ *iter };
        if(std::experimental::filesystem::exists(filePath) && filePath.has_extension())
        {
            std::string extension = filePath.extension().string();

            if(extension == ".slgBinPos2")
            {
                if(m_version == CloudMesh::Version::V1)
                {
                    throw std::exception{ SLG_NORMALIZE_EXCEPTION_MESSAGE("You create a cloud mesh using different version cloud files") };
                }

                m_version = CloudMesh::Version::V2;
            }
            else if(extension == ".slgBinPos")
            {
                if(m_version == CloudMesh::Version::V2)
                {
                    throw std::exception{ SLG_NORMALIZE_EXCEPTION_MESSAGE("You create a cloud mesh using different version cloud files") };
                }

                m_version = CloudMesh::Version::V1;
            }
            else
            {
                throw std::exception{ SLG_NORMALIZE_EXCEPTION_STRING_MESSAGE("Unsupported cloud file : " + *iter) };
            }

            m_cloudFilesName.emplace_back(*iter);
        }
        else
        {
            throw std::exception{ SLG_NORMALIZE_EXCEPTION_STRING_MESSAGE("Invalid cloud file : " + *iter) };
        }
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

            this->setBuffers(immediateContext, sizeof(VertexType), 0);

            const PreInitializeCBufferParameterFromMeshInstance& thisMeshParameters = this->getMeshParams();

            ConstantBufferParameterHelper::transfer(thisMeshParameters, renderCBufferParameter);

            /*Those matrix are transposed : transpose(A * B) == transpose(B) * transpose(A) */
            renderCBufferParameter.m_TransposedMatWorldViewProj = preInitShadingCBuffer.m_TransposedMatViewProj * thisMeshParameters.m_TransposedMatWorld;

            UINT indexCount = static_cast<UINT>(m_cloud.m_indexes.size());

            auto endIter = m_materialArray.end();
            for(auto iter = m_materialArray.begin(); iter != endIter; ++iter)
            {
                iter->prepareDraw(immediateContext, renderCBufferParameter);

                immediateContext->DrawIndexed(indexCount, 0, 0);
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
        [this, version = this->m_version]()
    {
        std::vector<Slg3DScanner::InternalCloudMesh> cloudDataResults;
        std::vector<std::future<Slg3DScanner::InternalCloudMesh>> readFutures;

        const std::size_t filesCount = m_cloudFilesName.size();
        readFutures.reserve(filesCount);
        cloudDataResults.reserve(filesCount);

        std::for_each(
            m_cloudFilesName.begin(),
            m_cloudFilesName.end(), 
            [version, &readFutures](const std::string& cloudFileName)
        {
            readFutures.emplace_back(std::async([cloudFileName, version]()
            {
                Slg3DScanner::InternalCloudMesh result;

                switch(version)
                {
                case CloudMesh::Version::V2:
                    Slg3DScanner::readPointCloud2fromFile(cloudFileName, result);
                    break;

                case CloudMesh::Version::V1:
                    result.m_vertexes = Slg3DScanner::readPointCloudfromFile(cloudFileName);
                    break;

                default:
                    throw std::exception{ SLG_NORMALIZE_EXCEPTION_MESSAGE("Unknown cloud files version") };
                }
                
                return std::move(result);
            }));
        });

        std::mutex readMutex;
        std::for_each(
            readFutures.begin(),
            readFutures.end(),
            [this, &readMutex](std::future<Slg3DScanner::InternalCloudMesh>& readFuture)
        {
            std::lock_guard<std::mutex> autoLocker{ readMutex };
            if(m_cloud.isEmpty())
            {
                m_cloud = std::move(readFuture.get());
            }
            else
            {
                Slg3DScanner::CloudMerger::merge(m_cloud, readFuture.get());
            }
        });

        this->internalSendDataToGraphicCard();

        m_initialized = true;
    } }.detach();
}

void CloudMesh::internalSendDataToGraphicCard()
{
    const Slg3DScanner::DXDispositif& d3d11GlobalDevice = RenderEngineManager::instance().getDevice();

    ID3D11Device* device = d3d11GlobalDevice.getD3DDevice();

    D3D11_BUFFER_DESC bufferDesc;
    D3D11_SUBRESOURCE_DATA initData;

    DXZeroMemory(
        bufferDesc,
        initData
    );
    
    UINT vertexCount = static_cast<UINT>(m_cloud.m_vertexes.size());

    bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    bufferDesc.StructureByteStride = sizeof(VertexType);
    bufferDesc.ByteWidth = vertexCount * bufferDesc.StructureByteStride;
    bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    initData.pSysMem = m_cloud.m_vertexes.data();

    DXTry(device->CreateBuffer(&bufferDesc, &initData, &m_vertexBuffer));

    if(m_version == CloudMesh::Version::V2 && CloudMesh::IGNORE_INDEX_BUFFER == false)
    {
        bufferDesc.StructureByteStride = sizeof(decltype(m_cloud.m_indexes)::value_type);
        bufferDesc.ByteWidth = static_cast<UINT>(m_cloud.m_indexes.size() * bufferDesc.StructureByteStride);
        bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;

        initData.pSysMem = static_cast<const void*>(m_cloud.m_indexes.data());
    }
    else
    {
        std::unique_ptr<UINT[]> indexBuffer = std::make_unique<UINT[]>(vertexCount);
        for(unsigned int iter = 0; iter != vertexCount; ++iter)
        {
            indexBuffer[iter] = iter;
        }

        bufferDesc.ByteWidth = vertexCount * sizeof(UINT);
        bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;

        initData.pSysMem = indexBuffer.get();
    }
    
    DXTry(device->CreateBuffer(&bufferDesc, &initData, &m_indexBuffer));
}
