#include "Lit.hlsli"

VertexPosHTex VS_2D(VertexPT vIn)
{
    VertexPosHTex vOut;
    vOut.PosH = float4(vIn.PosL, 1.0f);
    vOut.Tex = vIn.Tex;
    return vOut;
}