#include "../LightsHeader.hlsli"
Texture2D m_texture : register(t0);
SamplerState m_samplerState : register(s0);

cbuffer OnRenderConstantBuffer : register(b0)
{
    matrix World;
    matrix WorldInverseTranspose;
}

cbuffer OnFrameConstantBuffer : register(b1)
{
    matrix View;
    float3 EyePos;
    matrix Proj;
}

cbuffer LightConstantBuffer : register(b2)
{
    DirectionalLight m_dirLight[10];
    PointLight m_pointLight[10];
    SpotLight m_spotLight[10];
    Material m_material;
    int NumOfDirLight;
    int NumOfPointLight;
    int NumOfSpotLight;
}

//cbuffer VSConstantBuffer : register(b0)
//{
//    matrix World; 
//    matrix View;  
//    matrix Proj;  
//    matrix WorldInverseTranspose;
//}

//cbuffer PSConstantBuffer : register(b1)
//{
//    DirectionalLight m_dirLight[10];
//    PointLight m_pointLight[10];
//    SpotLight m_spotLight[10];
//    Material m_material;
//    int NumOfDirLight;
//    int NumOfPointLight;
//    int NumOfSpotLight;
//    float dummy1;

//    float3 EyePos;
//    float dummy2;
//}


struct VertexPNT
{
    float3 posL : POSITION;
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
    float4 posH : SV_POSITION;
    float3 posW : POSITION;   
    float3 normal : NORMAL;  
    float2 tex : TEXCOORD;
};

struct VertexPhT
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD;
};