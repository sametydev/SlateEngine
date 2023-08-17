#pragma once
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Component/Material.h>
#include <SlateEngine/Engine/Component/DirectionalLight.h>
#include <SlateEngine/Engine/Component/SpotLight.h>
#include <SlateEngine/Engine/Component/PointLight.h>


struct VertexBufferDesc {
    const void* pData;
    UINT cbSize;
    UINT cbStride;
};

struct IndexBufferDesc {
    const void* pData;
    UINT    cbSize;
};

struct ConstantBufferDesc {
    void* pData;
    UINT    cbSize;
};


struct ObjectConstantBuffer
{
    mat4x4 world;
    mat4x4 worldInverseTranspose;
};

struct FrameConstantBuffer
{
    mat4x4 view;
    vec3f eyePos;
    mat4x4 proj;
};

struct LightConstantBuffer
{
    DirectionalLight dirLight[10];
    PointLight pointLight[10];
    SpotLight spotLight[10];
    Material material;
    int numDirLight;
    int numPointLight;
    int numSpotLight;
    float dummy;
};

class DXBuffer {
public:
    DXBuffer(){}
    virtual~DXBuffer(){}

    virtual void Reset() = 0;
    virtual void BindPipeline(UINT offset) = 0;

private:
    ComPtr<ID3D11Buffer> m_buffer;
};