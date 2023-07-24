#pragma once
#include <TestEngine/Engine/DXConfig.h>

struct PointLight
{
    PointLight() { memset(this, 0, sizeof(PointLight)); }

    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular;

    DirectX::XMFLOAT3 position;
    float range;

    DirectX::XMFLOAT3 attenutation;
    float pad;
};