#include "Camera.h"

#include "RenderSceneManager.h"

using namespace Slg3DScanner;


Camera::Camera(const CameraParameters& parameters) :
    m_Position{ 0.f, 0.f, 0.f, 1.f },
    m_MatView{ DirectX::XMMatrixIdentity() },
    m_isRegistered{ false }
{
    this->setCameraParameters(parameters);
}

Camera::~Camera() = default;

void Camera::setCameraParameters(const CameraParameters& parameter)
{
    m_Parameters = parameter;

    m_MatProj = DirectX::XMMatrixPerspectiveFovRH(m_Parameters.fovYAngle, m_Parameters.aspectRatio, m_Parameters.nearPlane, m_Parameters.farPlane);

    this->updateTransposedMatViewProj();
}

void Camera::setMatView(const DirectX::XMMATRIX& matView)
{
    m_MatView = matView;

    this->updateTransposedMatViewProj();
}

void Camera::setPosition(const DirectX::XMVECTOR& newPosition)
{
    m_Position = newPosition;
}

void Camera::setDirection(const DirectX::XMVECTOR& newDirection)
{
    m_Direction = newDirection;
}

void Camera::setUp(const DirectX::XMVECTOR& newUp)
{
    m_Up = newUp;
}

void Camera::updateTransposedMatViewProj()
{
    m_TransposedMatViewProj = DirectX::XMMatrixTranspose(m_MatView * m_MatProj);
}

void Camera::registerComponent()
{
    m_isRegistered = true;
}

void Camera::unregisterComponent()
{
    m_isRegistered = false;
}

bool Camera::isRegistered() const
{
    return m_isRegistered;
}

void Camera::destroy()
{
    this->unregisterComponent();
}

void Camera::update(float deltaTime)
{

}
