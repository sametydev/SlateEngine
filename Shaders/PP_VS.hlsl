struct VSInput
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

PSInput VS(VSInput input)
{
    PSInput output;
    output.position = float4(input.position, 1.0);
    output.texcoord = input.texcoord;
    return output;
}
