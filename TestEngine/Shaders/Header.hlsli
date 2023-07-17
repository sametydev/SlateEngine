
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Proj;
}

struct VERTEX_IN
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VERTEX_OUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};