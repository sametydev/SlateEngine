#pragma once
#include <SlateEngine/Engine/DXConfig.h>

struct PointLight
{
    PointLight() = default;

    PointLight(const PointLight&) = default;
    PointLight& operator=(const PointLight&) = default;

    PointLight(PointLight&&) = default;
    PointLight& operator=(PointLight&&) = default;

    PointLight(const DirectX::XMFLOAT4& _ambient, const DirectX::XMFLOAT4& _diffuse, const DirectX::XMFLOAT4& _specular,
        const DirectX::XMFLOAT3& _position, float _range, const DirectX::XMFLOAT3& _att) :
        ambient(_ambient), diffuse(_diffuse), specular(_specular), position(_position), range(_range), attenutation(_att), dummy() {}


    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular;
    DirectX::XMFLOAT3 position;
    float range;
    DirectX::XMFLOAT3 attenutation;
    float dummy;
};