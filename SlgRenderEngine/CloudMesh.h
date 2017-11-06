#pragma once

#include "Mesh.h"

namespace Slg3DScanner
{
    struct CloudVertex;
    struct CloudMeshInitializer : public MeshInitializer
    {
        std::string m_cloudFileName;
        int m_version;
    };

    class CloudMesh : public Mesh
    {

    public:
        using VertexType = CloudVertex;


    private:
        std::string m_cloudFileName;

        std::vector<CloudVertex> m_cloud;
        D3D11_PRIMITIVE_TOPOLOGY m_cloudTopology;

        std::atomic<bool> m_initialized;

        int m_version;


    public:
        CloudMesh(const CloudMeshInitializer& cloudMeshInitializer);
        ~CloudMesh();

        virtual void draw(ID3D11DeviceContext* immediateContext, const PreInitializeCBufferParameterFromRendererSceneManager& preInitShadingCBuffer) override;


    public:
        void setCloudFile(const std::string &cloudFileName);
        void readCloudFile();
        void internalSendDataToGraphicCard();


    private:
        //no reference
        void internalSetCloudFile(std::string cloudFileName);
    };
}
