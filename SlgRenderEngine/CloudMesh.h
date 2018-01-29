#pragma once

#include "Mesh.h"

#include "CloudVertex.h"

namespace Slg3DScanner
{
    struct CloudVertex;
    struct CloudMeshInitializer : public MeshInitializer
    {
        std::vector<std::string> m_cloudFilesName;
    };

    class CloudMesh : public Mesh
    {
    public:
        using VertexType = CloudVertex;


    public:
        enum 
        {
            IGNORE_INDEX_BUFFER = false,
            
            DEFAULT_INDEX_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
            CURRENT_INDEX_TOPOLOGY = IGNORE_INDEX_BUFFER ? D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST : DEFAULT_INDEX_TOPOLOGY
        };

        enum Version
        {
            V1,
            V2,

            UNKNOWN
        };


    private:
        std::vector<std::string> m_cloudFilesName;

        Slg3DScanner::InternalCloudMesh m_cloud;
        D3D11_PRIMITIVE_TOPOLOGY m_cloudTopology;

        Version m_version;


    public:
        CloudMesh(const CloudMeshInitializer& cloudMeshInitializer);
        ~CloudMesh();

        virtual void draw(ID3D11DeviceContext* immediateContext, const PreInitializeCBufferParameterFromRendererSceneManager& preInitShadingCBuffer) override;


    private:
        void setCloudFile(const std::vector<std::string>& cloudFileName);
        void readCloudFile();
        void internalSendDataToGraphicCard();


    public:
        virtual bool writeToObj(const std::string& objPath) override;
    };
}
