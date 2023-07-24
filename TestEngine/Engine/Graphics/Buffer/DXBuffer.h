#pragma once
#include <TestEngine/Engine/DXConfig.h>
#include <TestEngine/Engine/Component/Material.h>
#include <TestEngine/Engine/Component/DirectionalLight.h>
#include <TestEngine/Engine/Component/SpotLight.h>
#include <TestEngine/Engine/Component/PointLight.h>

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

__declspec(align(16))
struct VS_ConstantBuffer
{
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX proj;
    DirectX::XMMATRIX worldInvTranspose;

};

__declspec(align(16))
struct PS_ConstantBuffer
{
    DirectionalLight dirLight;
    PointLight pointLight;
    SpotLight spotLight;
    Material material;
    DirectX::XMFLOAT4 eyePos;
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