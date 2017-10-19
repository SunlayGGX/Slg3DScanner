#pragma once

#include "TestSheetPrototype.h"

SLG_BEGIN_TEST_ORDER_DEFINITION(DrawCubes)
    const DirectX::XMVECTOR cameraPosition = { 0.f, 0.f, -10.f, 1.f };
    const DirectX::XMVECTOR cameraDirection = { 0.f, 0.f, 0.f, 1.f };
    const DirectX::XMVECTOR cameraUp = { 0.f, 1.f, 0.f, 0.f };

    const DirectX::XMFLOAT3 cube1Position = { -2.f, 2.f, -2.f };
    const DirectX::XMFLOAT3 cube2Position = { -2.f, -2.f, -2.f };
    const DirectX::XMFLOAT3 cube3Position = { 2.f, 2.f, -2.f };
    const DirectX::XMFLOAT3 cube4Position = { 2.f, -2.f, -2.f };
SLG_END_TEST_ORDER_DEFINITION


SLG_BEGIN_TEST_ORDER_DEFINITION(LoadPointCloud)
    const std::string pointCloudFilePath = "";
SLG_END_TEST_ORDER_DEFINITION
