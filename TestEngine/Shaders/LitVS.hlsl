#include "LightsHeader.hlsli"

VERTEX_OUT VS(VERTEX_IN _in)
{
    VERTEX_OUT vOut;
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(_in.PosL, 1.0f), g_World);

    vOut.PosH = mul(posW, viewProj);
    vOut.PosW = posW.xyz;
    vOut.Normal = mul(_in.Normal, (float3x3) g_WorldInvTranspose);
    vOut.Color = _in.Color;
    return vOut;
}