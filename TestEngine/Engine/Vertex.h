#pragma once
#include <TestEngine/Engine/DXConfig.h>

struct VertexPC
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;
    static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};