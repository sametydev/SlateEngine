#include "Lit.hlsli"
struct PSInput
{
    float4 position : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float4 color : COLOR;
};

float4 main(PSInput input) : SV_TARGET
{
    float gridScale = 1.0f;
    float lineThickness = 0.05f;

    float2 gridUV = input.worldPos.xz / gridScale;

    float gridLineX = abs(frac(gridUV.x) - 0.5f);
    float gridLineZ = abs(frac(gridUV.y) - 0.5f);

    float linex = step(lineThickness, gridLineX) * step(lineThickness, gridLineZ);

    float4 gridColor = lerp(float4(1.0f, 1.0f, 1.0f, 1.0f), float4(0.2f, 0.2f, 0.2f, 1.0f), linex);

    return gridColor * input.color;
}