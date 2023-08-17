#include <SlateEngine/Engine/Component/RenderableObject.h>

RenderableObject::RenderableObject()
{
    transform = new Transform();
    transform->SetPosition({ 0,0,0 });
    //Create our Vertex Buffer
    m_vertexBuffer = new DXVertexBuffer();

    //Create our Index Buffer
    m_indexBuffer = new DXIndexBuffer();

    m_objectConstantBuffer = new DXConstantBuffer();

    ObjectConstantBufferObject.world = mat4x4();
    ObjectConstantBufferObject.worldInverseTranspose = mat4x4();

    ConstantBufferDesc cbd{};
    cbd.cbSize = sizeof(ObjectConstantBuffer);
    m_objectConstantBuffer->Create(cbd);

    m_objectConstantBuffer->BindVS(0);
    m_objectConstantBuffer->BindPS(0);
}

RenderableObject::~RenderableObject()
{
}

vec3f RenderableObject::GetPosition() const
{
	return vec3f();
}

void RenderableObject::SetTexture(DXTexture* texture)
{
    texture->Bind(0);
}

void RenderableObject::SetWorldMatrix(const mat4x4& world)
{
}

void RenderableObject::OnUpdate(float deltaTime)
{
    
    //TEMPORARY!
    py += 10.f * deltaTime, tx += 10.f * deltaTime;
    transform->SetRotation({ py,tx,0 });
    ObjectConstantBufferObject.world = transform->GetGlobal();
    ObjectConstantBufferObject.worldInverseTranspose = mat4x4::transposed(transform->GetGlobal().inverted());
    transform->Update();
    //TEMPORARY!

    m_objectConstantBuffer->Map(sizeof(ObjectConstantBuffer), &ObjectConstantBufferObject);
    m_objectConstantBuffer->UnMap();
}

void RenderableObject::OnRender()
{
    DXApplication::Instance->GetDXContext().Get()->DrawIndexed(m_indices, 0, 0);
}
