#pragma once
#include <TestEngine/Engine/DXConfig.h>

struct Material
{
    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular;
    DirectX::XMFLOAT4 reflect;
};