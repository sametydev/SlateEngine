#include "Lit.hlsli"

VertexPhwNT main(VertexPNT vIn)
{
    VertexPhwNT vOut;
    matrix viewProj = mul(View, Proj);
    float4 posW = mul(float4(vIn.posL, 1.0f), World);

    vOut.posH = mul(posW, viewProj);
    vOut.posW = posW.xyz;
    vOut.normal = mul(vIn.normal, (float3x3) WorldInverseTranspose);
    vOut.tex = vIn.tex;
    return vOut;
}