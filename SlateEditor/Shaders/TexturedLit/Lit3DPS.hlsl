#include "Lit.hlsli"

float4 main(VertexPhwNT pIn) : SV_Target
{

    pIn.normal = normalize(pIn.normal);


    float3 toEye = normalize(EyePos - pIn.posWorld);

    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);
    int i = 0;
    
    for (i = 0; i < NumOfDirLight; ++i)
    {
        ComputeDirectionalLight(m_material, m_dirLight[i], pIn.normal, toEye, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }
    
    for (i = 0; i < NumOfPointLight; ++i)
    {
        ComputePointLight(m_material, m_pointLight[i], pIn.posWorld, pIn.normal, toEye, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }
    
    for (i = 0; i < NumOfSpotLight; ++i)
    {
        ComputeSpotLight(m_material, m_spotLight[i], pIn.posWorld, pIn.normal, toEye, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }
    
    float4 texColor = m_texture.Sample(m_samplerState, pIn.tex);
    float4 litColor = texColor * (ambient + diffuse) + spec;
    litColor.a = texColor.a * m_material.diffuse.a;
    
    return litColor;
}