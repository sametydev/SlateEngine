#pragma once
#include <TestEngine/Engine/DXConfig.h>

//P = POS
//C = COLOR
//N = NORMAL
//T = TEXTURE
//S = SIZE

struct VertexPNC
{
    VertexPNC() = default;

    VertexPNC(const VertexPNC&) = default;
    VertexPNC& operator=(const VertexPNC&) = default;

    VertexPNC(VertexPNC&&) = default;
    VertexPNC& operator=(VertexPNC&&) = default;

    constexpr VertexPNC(const DirectX::XMFLOAT3& _pos, const DirectX::XMFLOAT3& _normal,
        const DirectX::XMFLOAT4& _color) :
        pos(_pos), normal(_normal), color(_color) {}

    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT4 color;
    static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
};


struct VertexPNT
{
    VertexPNT() = default;

    VertexPNT(const VertexPNT&) = default;
    VertexPNT& operator=(const VertexPNT&) = default;

    VertexPNT(VertexPNT&&) = default;
    VertexPNT& operator=(VertexPNT&&) = default;

    constexpr VertexPNT(const DirectX::XMFLOAT3& _pos, const DirectX::XMFLOAT3& _normal,
        const DirectX::XMFLOAT2& _tex) :
        pos(_pos), normal(_normal), tex(_tex) {}

    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 tex;
    static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
};