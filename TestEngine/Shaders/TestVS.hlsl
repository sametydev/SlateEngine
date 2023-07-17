#include "Header.hlsli"

VERTEX_OUT VS(VERTEX_IN vs_in)
{
    VERTEX_OUT vs_out;
    vs_out.pos = mul(float4(vs_in.pos, 1.0f), World);
    vs_out.pos = mul(vs_out.pos, View);
    vs_out.pos = mul(vs_out.pos, Proj);
    vs_out.color = vs_in.color;
    return vs_out;
}