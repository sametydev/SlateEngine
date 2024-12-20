#pragma once
#include <SlateEngine/Engine/DXConfig.h>

//P = POS
//C = COLOR
//N = NORMAL
//T = TEXTURE
//S = SIZE


struct ENGINE_API VertexPT
{
    VertexPT() = default;

    VertexPT(const VertexPT&) = default;
    VertexPT& operator=(const VertexPT&) = default;

    VertexPT(VertexPT&&) = default;
    VertexPT& operator=(VertexPT&&) = default;

    VertexPT(const vec3f& _pos, const vec2f& _tex) :
        pos(_pos), tex(_tex) {}

    vec3f pos{};
    vec2f tex{};
    static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

struct ENGINE_API VertexPC
{
    VertexPC() = default;

    VertexPC(const VertexPC&) = default;
    VertexPC& operator=(const VertexPC&) = default;

    VertexPC(VertexPC&&) = default;
    VertexPC& operator=(VertexPC&&) = default;

    VertexPC(const vec3f& _pos, const vec4f& _col) :
        pos(_pos), color(_col) {
    }

    vec3f pos{};
    vec4f color{};
    static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

struct ENGINE_API VertexPNC
{
    VertexPNC() = default;

    VertexPNC(const VertexPNC&) = default;
    VertexPNC& operator=(const VertexPNC&) = default;

    VertexPNC(VertexPNC&&) = default;
    VertexPNC& operator=(VertexPNC&&) = default;

    VertexPNC(const vec3f& _pos, const vec3f& _normal,
        const vec4f& _color) :
        pos(_pos), normal(_normal), color(_color) {}

    vec3f pos;
    vec3f normal;
    vec4f color;
    static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
};


struct ENGINE_API VertexPNT
{
    VertexPNT() = default;

    VertexPNT(const VertexPNT&) = default;
    VertexPNT& operator=(const VertexPNT&) = default;

    VertexPNT(VertexPNT&&) = default;
    VertexPNT& operator=(VertexPNT&&) = default;

    VertexPNT(const vec3f& _pos, const vec3f& _normal,
        const vec2f& _tex) :
        pos(_pos), normal(_normal), tex(_tex) {}

    vec3f pos;
    vec3f normal;
    vec2f tex;
    static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
};