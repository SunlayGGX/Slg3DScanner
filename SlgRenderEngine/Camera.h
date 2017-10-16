#pragma once

#include "UtilsMacros.h"
#include "FacetteAttribute.h"

#include "Component.h"

namespace Slg3DScanner
{
    class RenderEngineManager;
    class RenderSceneManager;

    struct CameraParameters
    {
        float fovYAngle;
        float aspectRatio;
        float nearPlane;
        float farPlane;
    };


    class Camera : 
        public Component,
        virtual public FacetteAttribute<Camera, Component>
    {
    private:
        friend RenderEngineManager;
        friend RenderSceneManager;


    private:
        CameraParameters m_Parameters;

        DirectX::XMVECTOR m_Position;

        DirectX::XMMATRIX m_MatView;
        DirectX::XMMATRIX m_MatProj;

        DirectX::XMMATRIX m_TransposedMatViewProj;

        std::atomic<bool> m_isRegistered;


    public:
        Camera(const CameraParameters& parameters);
        ~Camera();


    public:
        DECLARE_SIMPLE_GET_REFERENCE_ACCESSOR(DirectX::XMVECTOR, Position);
        DECLARE_SIMPLE_GET_REFERENCE_ACCESSOR(DirectX::XMMATRIX, MatView);
        DECLARE_SIMPLE_GET_REFERENCE_ACCESSOR(DirectX::XMMATRIX, MatProj);
        DECLARE_SIMPLE_GET_REFERENCE_ACCESSOR(CameraParameters, Parameters);

        /*Get the transposed matrix of matviewproj*/
        DECLARE_SIMPLE_GET_REFERENCE_ACCESSOR(DirectX::XMMATRIX, TransposedMatViewProj);

        void setCameraParameters(const CameraParameters& parameter);
        void setMatView(const DirectX::XMMATRIX& matView);

        virtual bool isRegistered() const override;

        virtual void destroy() override;

        virtual void update(float deltaTime);


    private:
        virtual void registerComponent() override;
        virtual void unregisterComponent() override;


    private:
        void updateTransposedMatViewProj();
    };
}

