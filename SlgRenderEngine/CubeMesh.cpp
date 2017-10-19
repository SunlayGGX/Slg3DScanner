#include "CubeMesh.h"

#include "DirectXUtilitary.h"

#include "RenderEngineManager.h"

#include "Material.h"
#include "Vertex.h"


using namespace Slg3DScanner;


CubeMesh::CubeMesh(const CubeMeshInitializer& cubeMeshInitializer) :
    NamedObject{ cubeMeshInitializer.name },
    Mesh{ cubeMeshInitializer }
{
    ID3D11Device* device = RenderEngineManager::instance().getDevice().getD3DDevice();

    D3D11_BUFFER_DESC bufferDesc;
    D3D11_SUBRESOURCE_DATA initData;

    DXZeroMemory(
        bufferDesc,
        initData
    );

    {
        /***********************************************************/
        //Vertex Buffer

        const float posHalfX = cubeMeshInitializer.xScale / 2.f;
        const float negHalfX = -posHalfX;
        const float posHalfY = cubeMeshInitializer.yScale / 2.f;
        const float negHalfY = -posHalfY;
        const float posHalfZ = cubeMeshInitializer.zScale / 2.f;
        const float negHalfZ = -posHalfZ;

        const DirectX::XMFLOAT3 vertexex[CubeMesh::VERTEX_ABSOLUTE_COUNT] =
        {
            { negHalfX, negHalfY, negHalfZ },
            { negHalfX, negHalfY, posHalfZ },
            { posHalfX, negHalfY, posHalfZ },
            { posHalfX, negHalfY, negHalfZ },
            { negHalfX, posHalfY, negHalfZ },
            { negHalfX, posHalfY, posHalfZ },
            { posHalfX, posHalfY, posHalfZ },
            { posHalfX, posHalfY, negHalfZ }
        };

        constexpr const DirectX::XMFLOAT3 normals[CubeMesh::FACE_COUNT] =
        {
            { 1.f, 0.f, 0.f },  //x pos -> 0
            { -1.f, 0.f, 0.f }, //x neg -> 1
            { 0.f, 1.f, 0.f },  //y pos -> 2
            { 0.f, -1.f, 0.f }, //y neg -> 3
            { 0.f, 0.f, 1.f },  //z pos -> 4
            { 0.f, 0.f, -1.f }  //z neg -> 5
        };

        constexpr const DirectX::XMFLOAT2 uvs[CubeMesh::VERTEX_PER_FACE] =
        {
            { 0.f, 0.f }, //down left   -> 0
            { 0.f, 1.f }, //up left     -> 1
            { 1.f, 1.f }, //up right    -> 2
            { 1.f, 0.f }  //down right  -> 3
        };

        const VertexType vertexBuffer[CubeMesh::VERTEX_TOTAL_COUNT] =
        {
            { vertexex[0], normals[3], uvs[0] }, // -> 0
            { vertexex[1], normals[3], uvs[1] }, // -> 1
            { vertexex[2], normals[3], uvs[2] }, // -> 2
            { vertexex[3], normals[3], uvs[3] }, // -> 3

            { vertexex[0], normals[1], uvs[3] }, // -> 4
            { vertexex[1], normals[1], uvs[2] }, // -> 5
            { vertexex[5], normals[1], uvs[1] }, // -> 6
            { vertexex[4], normals[1], uvs[0] }, // -> 7

            { vertexex[0], normals[5], uvs[1] }, // -> 8
            { vertexex[3], normals[5], uvs[2] }, // -> 9
            { vertexex[7], normals[5], uvs[3] }, // -> 10
            { vertexex[4], normals[5], uvs[0] }, // -> 11

            { vertexex[3], normals[0], uvs[0] }, // -> 12
            { vertexex[2], normals[0], uvs[1] }, // -> 13
            { vertexex[6], normals[0], uvs[2] }, // -> 14
            { vertexex[7], normals[0], uvs[3] }, // -> 15

            { vertexex[1], normals[4], uvs[0] }, // -> 16
            { vertexex[5], normals[4], uvs[1] }, // -> 17
            { vertexex[6], normals[4], uvs[2] }, // -> 18
            { vertexex[2], normals[4], uvs[3] }, // -> 19

            { vertexex[4], normals[2], uvs[3] }, // -> 20
            { vertexex[5], normals[2], uvs[2] }, // -> 21
            { vertexex[6], normals[2], uvs[1] }, // -> 22
            { vertexex[7], normals[2], uvs[0] }  // -> 23
        };

        bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = CubeMesh::VERTEX_TOTAL_COUNT * sizeof(VertexType);
        bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;

        initData.pSysMem = vertexBuffer;

        DXTry(device->CreateBuffer(&bufferDesc, &initData, &m_vertexBuffer));
    }
    


    /*************************************************************/
    //Index Buffer
    constexpr const TriangleIndex indexBuffer[CubeMesh::TRIANGLE_COUNT] =
    {
        { 0,  1,  2  },
        { 0,  2,  3  },

        { 7,  6,  5  },
        { 7,  5,  4  },

        { 11, 8,  9  },
        { 11, 9,  10 },

        { 12, 13, 14 },
        { 12, 14, 15 },

        { 16, 17, 18 },
        { 16, 18, 19 },

        { 23, 22, 21 },
        { 23, 21, 20 }
    };

    bufferDesc.ByteWidth = CubeMesh::TRIANGLE_COUNT * sizeof(TriangleIndex);
    bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;

    initData.pSysMem = indexBuffer;

    DXTry(device->CreateBuffer(&bufferDesc, &initData, &m_indexBuffer));

    this->setBuffers(RenderEngineManager::instance().getDevice().getImmediateContext(), sizeof(VertexType), 0);
}

CubeMesh::~CubeMesh()
{
}

void CubeMesh::draw(ID3D11DeviceContext* immediateContext, const PreInitializeCBufferParameterFromRendererSceneManager& preInitShadingCBuffer)
{
    ForwardShadingConstantBufferParameter renderCBufferParameter;

    ConstantBufferParameterHelper::transfer(preInitShadingCBuffer, renderCBufferParameter);

    std::lock_guard<std::mutex> autoLocker{ m_mutex };

    const PreInitializeCBufferParameterFromMeshInstance& thisMeshParameters = this->getMeshParams();

    ConstantBufferParameterHelper::transfer(thisMeshParameters, renderCBufferParameter);

    /*Those matrix are transposed : transpose(A * B) == transpose(B) * transpose(A) */
    renderCBufferParameter.m_TransposedMatWorldViewProj = preInitShadingCBuffer.m_TransposedMatViewProj * thisMeshParameters.m_TransposedMatWorld;

    auto endIter = m_materialArray.end();
    for(auto iter = m_materialArray.begin(); iter != endIter; ++iter)
    {
        iter->prepareDraw(immediateContext, renderCBufferParameter);

        immediateContext->DrawIndexed(CubeMesh::INDEX_COUNT, 0, 0);
    }
}