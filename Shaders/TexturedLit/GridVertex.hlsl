#include "Lit.hlsli"

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput main(VertexPC input)
{
    PSInput output;

    float4 worldPosition = float4(input.position, 1.0f);
    worldPosition = mul(worldPosition, World);

    worldPosition = mul(worldPosition, View);

    output.position = mul(worldPosition, Proj);

    output.color = input.color;
    return output;
}