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
        void update();


    public:
        void createCamera(const CameraParameters& cameraParameters);
        void destroyCamera();

        DirectX::XMVECTOR getMainCameraPosition() const;
        DirectX::XMMATRIX getMainCameraTransposedMatViewProj() const;
        void setMainCameraMatViewManually(const DirectX::XMVECTOR& eyePosition, const DirectX::XMVECTOR& focusDirection, const DirectX::XMVECTOR& upDirection);


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

            if(indexFound == m_meshArray.size()) //not found, we create it
            {
                m_meshArray.emplace_back(std::make_shared<MeshType>(initializer));
            }

            return m_meshArray[indexFound];
        }
    };
}


