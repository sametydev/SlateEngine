Texture2D sceneTexture : register(t0);
SamplerState samplerState : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 PS(PSInput input) : SV_Target
{
    return sceneTexture.Sample(samplerState, input.texcoord);
}
