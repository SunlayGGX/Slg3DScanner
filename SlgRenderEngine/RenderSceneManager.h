#pragma once

#include "SlgSingleton.h"
#include "FacetteAttribute.h"
#include "GlobalDef.h"

#include "IMesh.h"



namespace Slg3DScanner
{
    struct CameraParameters;
    DECLARE_SLG_SINGLETON_ALLOCATOR_HELPER_FOR(RenderSceneManager)

    class RenderSceneManager : private SlgSingleton<RenderSceneManager>
    {
    private:
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(RenderSceneManager);


    private:
        mutable std::mutex m_mutex;

        IMeshShArray m_meshArray;
        CameraRef m_currentPlayerCamera;


    public:
        virtual void initialize() override;
        virtual void destroy() override;

        void update();


    public:
        void createCamera(const CameraParameters& cameraParameters);
        void destroyCamera();

        DirectX::XMVECTOR getMainCameraPosition() const;
        DirectX::XMMATRIX getMainCameraTransposedMatViewProj() const;


    private:
        void updateCamera(float deltaTime);
        void registerMainCamera(CameraRef newMainCamera);
        void unregisterMainCamera();


    public:
        IMeshRef findMeshByName(const std::string& meshName) const;


    private:
        std::size_t internalFindIndexOfMeshByName(const std::string& meshName) const;


    public:
        template<class MeshType, class MeshInitializer>
        IMeshRef createMeshOfType(const MeshInitializer& initializer)
        {
            std::lock_guard<std::mutex> autoLocker{ m_mutex };

            std::size_t indexFound = this->internalFindIndexOfMeshByName(initializer.name);

            if(indexFound == m_MeshArray.size()) //not found, we create it
            {
                m_MeshArray.emplace_back(std::make_shared<MeshType>(initializer));
            }

            return m_meshArray[indexFound];
        }
    };
}


