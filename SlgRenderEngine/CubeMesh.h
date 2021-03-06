#pragma once

#include "Mesh.h"

namespace Slg3DScanner
{
    struct SimpleVertex;
    struct PreInitializeCBufferParameterFromRendererSceneManager;

    struct CubeMeshInitializer : public MeshInitializer
    {
        float xScale = 1.f; 
        float yScale = 1.f; 
        float zScale = 1.f;
    };

    class CubeMesh : public Mesh
    {
    public:
        using VertexType = SimpleVertex;


    public:
        enum
        {
            VERTEX_PER_FACE = 4,
            VERTEX_ABSOLUTE_COUNT = 8,

            FACE_COUNT = 6,

            VERTEX_TOTAL_COUNT = FACE_COUNT * 4,

            TRIANGLE_COUNT = FACE_COUNT * 2,
            INDEX_COUNT = TRIANGLE_COUNT * 3
        };


    public:
        CubeMesh(const CubeMeshInitializer& cubeMeshInitializer);
        ~CubeMesh();

        virtual void draw(ID3D11DeviceContext* immediateContext, const PreInitializeCBufferParameterFromRendererSceneManager& preInitShadingCBuffer) override;
    };
}

