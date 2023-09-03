#pragma once
#pragma once
#include <SlateEngine/Engine/Graphics/Vertex.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXBuffer.h>
#include <SlateEngine/Engine/Graphics/Shader/DXVertexShader.h>
#include <SlateEngine/Engine/Graphics/Shader/DXPixelShader.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXVertexBuffer.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXIndexBuffer.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXConstantBuffer.h>
#include <SlateEngine/Engine/Graphics/BuiltInMesh.h>
#include <SlateEngine/Engine/Graphics/Texture/DXTexture.h>
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include <SlateEngine/Engine/Component/Transform.h>
#include <SlateEngine/Engine/Entity/Entity.h>
#include <SlateEngine/Engine/Component/Component.h>

struct RenderableGeometry : public Component {
public:
    RenderableGeometry() = default;
    ~RenderableGeometry() {};

    void OnInternalInit() override;

    template<class VertexType, class IndexType>
    void SetBuffer(const MeshData<VertexType, IndexType>& meshData);
    void SetTexture(DXTexture* texture);
    void OnUpdate(float deltaTime) override;
    void OnRender() override;

    Transform& GetTransform() { return connectedEntity->GetComponent<Transform>(); };
    ObjectConstantBuffer& GetObjectCb() { return cbData; };
    MaterialComponent& GetMaterial() { return m_material; };

private:
    MaterialComponent m_material{};

    DXVertexBuffer* m_vertexBuffer           = nullptr;
    DXIndexBuffer* m_indexBuffer             = nullptr;

    ObjectConstantBuffer   cbData{};

    DXConstantBuffer* m_objectConstantBuffer = nullptr;

    DXVertexShader* m_vertexShader           = nullptr;
    DXPixelShader* m_pixelShader             = nullptr;

    UINT m_indices                           = 0;

    DXTexture* attachedTexture = nullptr;

};

template<class VertexType, class IndexType>
inline void RenderableGeometry::SetBuffer(const MeshData<VertexType, IndexType>& meshData)
{
    //Reset old buffers
    m_vertexBuffer->Reset();
    m_indexBuffer->Reset();

    //Creating Vertex Buffer
    VertexBufferDesc vbd{};
    vbd.cbSize   = (UINT)meshData.vVertex.size() * sizeof(VertexPNT);
    vbd.cbStride = sizeof(VertexPNT);
    vbd.pData    = meshData.vVertex.data();
    m_vertexBuffer->Create(vbd);
    m_vertexBuffer->BindPipeline(0);

    //Storing indices count
    m_indices    = (UINT)meshData.vIndices.size();

    //Creating Index Buffer
    IndexBufferDesc ibd{};
    ibd.cbSize   = m_indices * sizeof(DWORD);
    ibd.pData    = meshData.vIndices.data();
    m_indexBuffer->Create(ibd);
    m_indexBuffer->BindPipeline(0);
}
