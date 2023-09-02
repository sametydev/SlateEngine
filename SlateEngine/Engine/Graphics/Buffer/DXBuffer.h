#pragma once
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Component/Material.h>
#include <SlateEngine/Engine/Component/Components.h>


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
    MaterialData material;
};

struct FrameConstantBuffer
{
    mat4x4 view;
    vec3f eyePos;
    mat4x4 proj;
};

struct LightConstantBuffer
{
    DirectionalLight dirLight[1];
    PointLight pointLight[1];
    SpotLight spotLight[1];
    int numDirLight = 0;
    int numPointLight = 1;
    int numSpotLight = 0;
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