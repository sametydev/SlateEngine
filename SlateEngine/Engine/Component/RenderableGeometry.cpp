#include <SlateEngine/Engine/Component/RenderableGeometry.h>
#include <SlateEngine/Engine/Graphics/Shader/ShaderCache.h>


void RenderableGeometry::OnInternalInit()
{
    mTransform = &GetEntity()->GetComponent<Transform>();
    this->SetBuffer(BuiltInMesh::CreateBox<VertexPNT>());

    m_constantBuffer = std::make_unique<DXConstantBuffer>();

    cbData.world = mat4x4();
    cbData.worldInverseTranspose = mat4x4();
    
    if (!GetEntity()->HasComponent<MaterialComponent>())
    {
        GetEntity()->AddComponent<MaterialComponent>();
    }
    
    m_material = &GetEntity()->GetComponent<MaterialComponent>();
    m_material->AddShader("Lit3DVS");
    m_material->AddShader("Lit3DPS");

    ConstantBufferDesc cbd{};
    cbd.cbSize = sizeof(ObjectConstantBuffer);
    m_constantBuffer->Create(cbd);

    //Calling Update once
    mat_ambient = m_material->GetPointer<vec4f>("$Ambient");
    mat_diff    = m_material->GetPointer<vec4f>("$Diffuse");
    mat_spec    = m_material->GetPointer<vec4f>("$Specular");
    OnUpdate(0);
}



void RenderableGeometry::OnUpdate(float deltaTime)
{
    cbData.world = mTransform->GetGlobal();
    cbData.worldInverseTranspose = mTransform->GetGlobal().InverseTranspose();

    cbData.material.ambient = *mat_ambient;
    cbData.material.diffuse = *mat_diff;
    cbData.material.specular = *mat_spec;

    m_constantBuffer->MapAndUnMap(sizeof(ObjectConstantBuffer), &cbData);
    //m_constantBuffer->SubData(&cbData);
}

void RenderableGeometry::OnRender(ID3D11DeviceContext* pDeviceContext)
{
    m_vertexBuffer->BindPipeline(0);
    m_indexBuffer->BindPipeline(0);
    m_constantBuffer->BindPipeline(0);

    m_material->SetIndices(m_indices);
}

void RenderableGeometry::OnInit()
{
}

void RenderableGeometry::OnShutdown()
{
}
