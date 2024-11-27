#pragma once
#include <SlateEngine/Engine/Graphics/Vertex.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXBuffer.h>
#include <SlateEngine/Engine/Graphics/Shader/DXVertexShader.h>
#include <SlateEngine/Engine/Graphics/Shader/DXPixelShader.h>
#include <SlateEngine/Engine/Graphics/Buffer/BufferCache.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXIndexBuffer.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXConstantBuffer.h>
#include <SlateEngine/Engine/Graphics/BuiltInMesh.h>
#include <SlateEngine/Engine/Graphics/Texture/DXTexture.h>
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include <SlateEngine/Engine/Component/Transform.h>
#include <SlateEngine/Engine/Entity/Entity.h>
#include <SlateEngine/Engine/Component/Component.h>

struct ENGINE_API RenderableGeometry : public Component {
public:
    RenderableGeometry() = default;
    ~RenderableGeometry() {};

    void OnInternalInit() override;

    template<class VertexType, class IndexType>
    void SetBuffer(const MeshData<VertexType, IndexType>& meshData);

    void OnUpdate(float deltaTime) override;
    void OnRender(ID3D11DeviceContext* pDeviceContext) override;

    void OnInit() override;
    void OnShutdown() override;

    Transform& GetTransform() { return GetEntity()->GetComponent<Transform>(); };
    ObjectConstantBuffer& GetObjectCb() { return cbData; };
    MaterialComponent& GetMaterial() { return *m_material; };

    const bool bStaticBuffer = false;
private:
    MaterialComponent* m_material;

    std::unique_ptr<DXVertexBuffer> m_vertexBuffer;
    std::unique_ptr<DXIndexBuffer> m_indexBuffer;
    
    ObjectConstantBuffer   cbData{};
    std::unique_ptr<DXConstantBuffer> m_constantBuffer;

    std::vector<DXBuffer*> buffers;

    UINT m_indices                           = 0;

    vec4f *mat_ambient, *mat_diff, *mat_spec;

    Transform* mTransform = nullptr;
};

template<class VertexType, class IndexType>
inline void RenderableGeometry::SetBuffer(const MeshData<VertexType, IndexType>& meshData)
{
    //Reset old buffers
    if(m_vertexBuffer)m_vertexBuffer->Reset();
    if(m_indexBuffer)m_indexBuffer->Reset();

    //Creating Vertex Buffer
    VertexBufferDesc vbd{};
    vbd.cbSize   = (UINT)meshData.vVertex.size() * sizeof(VertexPNT);
    vbd.cbStride = sizeof(VertexPNT);
    vbd.pData    = meshData.vVertex.data();
    if (m_vertexBuffer == nullptr)m_vertexBuffer = std::make_unique<DXVertexBuffer>();
    m_vertexBuffer->Create(vbd);
    m_vertexBuffer->BindPipeline(0);

    //Storing indices count
    m_indices    = (UINT)meshData.vIndices.size();

    //Creating Index Buffer
    IndexBufferDesc ibd{};
    ibd.cbSize   = m_indices * sizeof(DWORD);
    ibd.pData    = meshData.vIndices.data();
    if(m_indexBuffer == nullptr)m_indexBuffer = std::make_unique<DXIndexBuffer>();
    m_indexBuffer->Create(ibd);
    m_indexBuffer->BindPipeline(0);
}
