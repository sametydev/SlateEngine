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

struct RenderableObject {
public:
	RenderableObject();
	~RenderableObject();

    template<class VertexType, class IndexType>
    void SetBuffer(const MeshData<VertexType, IndexType>& meshData);
    void SetTexture(DXTexture* texture);
    void OnUpdate(float deltaTime);
    void OnRender();

    Transform& GetTransform() { return *transform; };
    ObjectConstantBuffer& GetObjectCb() { return ObjectConstantBufferObject; };

private:
    Transform* transform{};

    DXVertexBuffer* m_vertexBuffer = nullptr;
    DXIndexBuffer* m_indexBuffer = nullptr;

    ObjectConstantBuffer   ObjectConstantBufferObject;
    DXConstantBuffer* m_objectConstantBuffer = nullptr;

    UINT m_indices = 0;

};

template<class VertexType, class IndexType>
inline void RenderableObject::SetBuffer(const MeshData<VertexType, IndexType>& meshData)
{
    //Reset old buffers
    m_vertexBuffer->Reset();
    m_indexBuffer->Reset();

    //Creating Vertex Buffer
    VertexBufferDesc vbd{};
    vbd.cbSize = (UINT)meshData.vVertex.size() * sizeof(VertexPNT);
    vbd.cbStride = sizeof(VertexPNT);
    vbd.pData = meshData.vVertex.data();
    m_vertexBuffer->Create(vbd);
    m_vertexBuffer->BindPipeline(0);

    //Storing indices count
    m_indices = (UINT)meshData.vIndices.size();

    //Creating Index Buffer
    IndexBufferDesc ibd{};
    ibd.cbSize = m_indices * sizeof(DWORD);
    ibd.pData = meshData.vIndices.data();
    m_indexBuffer->Create(ibd);
    m_indexBuffer->BindPipeline(0);
}
