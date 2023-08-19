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

    ObjectConstantBufferObject.material.ambient = vec4f(0.5f, 0.5f, 0.5f, 1.0f);
    ObjectConstantBufferObject.material.diffuse = vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    ObjectConstantBufferObject.material.specular = vec4f(0.1f, 0.1f, 0.1f, 5.0f);

    ConstantBufferDesc cbd{};
    cbd.cbSize = sizeof(ObjectConstantBuffer);
    m_objectConstantBuffer->Create(cbd);

    m_objectConstantBuffer->BindVS(0);
    m_objectConstantBuffer->BindPS(0);
}

RenderableObject::~RenderableObject()
{
}
void RenderableObject::SetTexture(DXTexture* texture)
{
    texture->Bind(0);
}


void RenderableObject::OnUpdate(float deltaTime)
{
    ObjectConstantBufferObject.world = transform->GetGlobal();
    ObjectConstantBufferObject.worldInverseTranspose = mat4x4::transposed(transform->GetGlobal().inverted());
    transform->Update();

    m_objectConstantBuffer->Map(sizeof(ObjectConstantBuffer), &ObjectConstantBufferObject);
    m_objectConstantBuffer->UnMap();
}

void RenderableObject::OnRender()
{
    DXApplication::Instance->GetDXContext().Get()->DrawIndexed(m_indices, 0, 0);
}
