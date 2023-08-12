#include <SlateEngine/Engine/Component/RenderableObject.h>

RenderableObject::RenderableObject()
{
    //Create our Vertex Buffer
    m_vertexBuffer = new DXVertexBuffer();

    //Create our Index Buffer
    m_indexBuffer = new DXIndexBuffer();

    m_objectConstantBuffer = new DXConstantBuffer();

    ObjectConstantBufferObject.world = XMMatrixIdentity();
    ObjectConstantBufferObject.worldInverseTranspose = XMMatrixIdentity();

    ConstantBufferDesc cbd{};
    cbd.cbSize = sizeof(ObjectConstantBuffer);
    m_objectConstantBuffer->Create(cbd);

    m_objectConstantBuffer->BindVS(0);
    m_objectConstantBuffer->BindPS(0);
}

RenderableObject::~RenderableObject()
{
}

XMFLOAT3 RenderableObject::GetPosition() const
{
	return XMFLOAT3();
}

void RenderableObject::SetTexture(DXTexture* texture)
{
    texture->Bind(0);
}

void RenderableObject::SetWorldMatrix(const XMFLOAT4X4& world)
{
}

void RenderableObject::OnUpdate(float deltaTime)
{
    //TEMPORARY!
    py += 0.17f * deltaTime, tx += 0.27f * deltaTime;
    py = XMScalarModAngle(py);
    tx = XMScalarModAngle(tx);
    XMMATRIX W = XMMatrixRotationX(py) * XMMatrixRotationY(tx);
    ObjectConstantBufferObject.world = XMMatrixTranspose(W);
    ObjectConstantBufferObject.worldInverseTranspose = XMMatrixTranspose(InverseTranspose(W));
    //TEMPORARY!

    m_objectConstantBuffer->Map(sizeof(ObjectConstantBuffer), &ObjectConstantBufferObject);
    m_objectConstantBuffer->UnMap();
}

void RenderableObject::OnRender()
{
    DXApplication::Instance->GetDXContext().Get()->DrawIndexed(m_indices, 0, 0);
}
