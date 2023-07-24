
//DEPRECATED RIGHT NOW!
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View; 
    matrix Proj; 
    vector Color;
}

struct VERTEX_IN
{
    float3 PosL : POSITION;
    float3 Normal : NORMAL;
    float4 Color : COLOR;
};

struct VERTEX_OUT
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 Normal : NORMAL;
    float4 Color : COLOR;
};