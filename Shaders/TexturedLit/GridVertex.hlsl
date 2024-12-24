#include "Lit.hlsli"

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float4 color : COLOR;
};

VSOutput main(VertexPC input)
{
    VSOutput output;
    float4 worldPosition = mul(float4(input.position, 1.0f), World);
    output.position = mul(worldPosition, mul(View, Proj));
    output.worldPos = worldPosition.xyz;
    output.color = input.color;
    return output;
}