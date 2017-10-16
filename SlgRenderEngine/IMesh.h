#pragma once

#include "NamedObject.h"
#include "Component.h"
#include "FacetteAttribute.h"

#include "Material.h"


namespace Slg3DScanner
{
    struct PreInitializeCBufferParameterFromRendererSceneManager;
    class RenderSceneManager;

    struct IMeshInitializer
    {
    public:
        std::string name;


    public:
        IMeshInitializer() = default;
        ~IMeshInitializer() = default;
    };

    /*
    Base parent of all Meshes

    The architecture I chose for now is as follow :
    Because, it isn't a game engine, 1 mesh is 1 instance (I reconstruct meshes, thus, the probability for 1 mesh to be absolutly the copy of another is nonexistant...
    So, I chose to not follow a game engine design with mesh and mesh instance but merge the mesh entity with its instance...

    Maybe later, I'd change this point but for now, development costs time and I can't afford such...
    */
    class IMesh : 
        virtual protected NamedObject,
        public Component,
        virtual public FacetteAttribute<IMesh, Component>
    {
    public:
        enum
        {
            INITIAL_INSTANCE_REFERENCE_RESERVE_STARTUP = 32
        };


    protected:
        friend class RenderSceneManager;


    protected:
        ID3D11Buffer* m_vertexBuffer;
        ID3D11Buffer* m_indexBuffer;

        std::vector<Material> m_materialArray;

        mutable std::mutex m_mutex;


    protected:
        IMesh(const IMeshInitializer& other);
        virtual ~IMesh();


    public:
        virtual void addMaterial(const MaterialInitializer& materialInit);
        virtual void addMaterial(MaterialInitializer&& materialInit);


    public:
        virtual void destroy();

        virtual void draw(ID3D11DeviceContext* immediateContext, const PreInitializeCBufferParameterFromRendererSceneManager& preInitShadingCBuffer) = 0;
    };
}
