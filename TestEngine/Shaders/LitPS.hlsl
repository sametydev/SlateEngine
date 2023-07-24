#include "LightsHeader.hlsli"

float4 PS(VERTEX_OUT _in) : SV_Target
{
    _in.Normal = normalize(_in.Normal);

    float3 toEyeW = normalize(g_EyePosW - _in.PosW);

    float4 ambient, diffuse, spec;
    float4 A, D, S;
    ambient = diffuse = spec = A = D = S = float4(0.0f, 0.0f, 0.0f, 0.0f);

    ComputeDirectionalLight(g_Material, g_DirLight, _in.Normal, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    ComputePointLight(g_Material, g_PointLight, _in.PosW, _in.Normal, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    ComputeSpotLight(g_Material, g_SpotLight, _in.PosW, _in.Normal, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    float4 litColor = _in.Color * (ambient + diffuse) + spec;
    
    litColor.a = g_Material.Diffuse.a * _in.Color.a;
    
    return litColor;
}