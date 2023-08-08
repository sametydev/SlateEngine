#include <SlateEngine/Engine/Component/RenderableObject.h>

RenderableObject::RenderableObject()
{
    //Create our Vertex Buffer
    m_vertexBuffer = new DXVertexBuffer();

    //Create our Index Buffer
    m_indexBuffer = new DXIndexBuffer();

    m_renderConstantBuffer = new DXConstantBuffer();

    OnRenderConstantObject.world = XMMatrixIdentity();
    OnRenderConstantObject.worldInverseTranspose = XMMatrixIdentity();

    ConstantBufferDesc cbd{};
    cbd.cbSize = sizeof(OnRenderConstantBuffer);
    m_renderConstantBuffer->Create(cbd);
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
    OnRenderConstantObject.world = XMMatrixTranspose(W);
    OnRenderConstantObject.worldInverseTranspose = XMMatrixTranspose(InverseTranspose(W));
    //TEMPORARY!

    m_renderConstantBuffer->Map(sizeof(OnRenderConstantBuffer), &OnRenderConstantObject);
    m_renderConstantBuffer->UnMap();
}

void RenderableObject::OnRender()
{
    DXApplication::Instance->GetDXContext().Get()->DrawIndexed(m_indices, 0, 0);
}

void RenderableObject::ConstantBufferBind(UINT slot)
{
    m_renderConstantBuffer->BindVS(slot);
    m_renderConstantBuffer->BindPS(slot);
}
