#include "../LightsHeader.hlsli"
Texture2D m_texture : register(t0);
SamplerState m_samplerState : register(s0);

cbuffer ObjectConstantBuffer : register(b0)
{
    matrix World;
    matrix WorldInverseTranspose;
    Material m_material;
}

cbuffer OnFrameConstantBuffer : register(b1)
{
    matrix View;
    float3 EyePos;
    matrix Proj;
}

cbuffer LightConstantBuffer : register(b2)
{
    DirectionalLight m_dirLight[1];
    PointLight m_pointLight[1];
    SpotLight m_spotLight[1];

    int NumOfDirLight;
    int NumOfPointLight;
    int NumOfSpotLight;
}

struct VertexPNT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

struct VertexPT
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
};

struct VertexPhwNT
{
    float4 posHom : SV_POSITION;
    float3 posWorld : POSITION;   
    float3 normal : NORMAL;  
    float2 tex : TEXCOORD;
};

struct VertexPHT
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD;
};