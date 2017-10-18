#pragma once

namespace Slg3DScanner
{
    namespace DefaultCameraParameter
    {
        static constexpr const float DEFAULT_FOV            = DirectX::XM_PIDIV4;
        static constexpr const float DEFAULT_ASPECT_RATIO   = 1.8f;
        static constexpr const float DEFAULT_NEAR_PLANE     = 1.f;
        static constexpr const float DEFAULT_FAR_PLANE      = 30.f;
    }

    struct DefaultRenderCoefficientParameters
    {
        static constexpr const DirectX::XMVECTOR DEFAULT_MESH_AMBIENT_COEFFICIENT = DirectX::XMVECTOR{ 0.25f, 0.25f, 0.25f, 1.f };
        static constexpr const DirectX::XMVECTOR DEFAULT_MESH_DIFFUSE_COEFFICIENT = DirectX::XMVECTOR{ 0.4f, 0.4f, 0.4f, 1.f };
        static constexpr const DirectX::XMVECTOR DEFAULT_MESH_SPECULAR_COEFFICIENT = DirectX::XMVECTOR{ 0.25f, 0.25f, 0.25f, 1.f };

        static constexpr const DirectX::XMVECTOR DEFAULT_LIGHT_AMBIENT_COEFFICIENT = DirectX::XMVECTOR{ 0.3f, 0.3f, 0.3f, 1.f };
        static constexpr const DirectX::XMVECTOR DEFAULT_LIGHT_DIFFUSE_COEFFICIENT = DirectX::XMVECTOR{ 0.4f, 0.4f, 0.4f, 1.f };
        static constexpr const DirectX::XMVECTOR DEFAULT_LIGHT_SPECULAR_COEFFICIENT = DirectX::XMVECTOR{ 0.3f, 0.3f, 0.3f, 1.f };

        static constexpr const DirectX::XMMATRIX DEFAULT_MAT_WORLD_AT_CREATION = DirectX::XMMATRIX
        {
            { 1.f, 0.f, 0.f, 0.f },
            { 0.f, 1.f, 0.f, 0.f },
            { 0.f, 0.f, 1.f, 0.f },
            { 0.f, 0.f, 0.f, 1.f }
        };
    };
}