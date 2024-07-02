#include <SlateEngine/Engine/Component/RenderableGeometry.h>
#include <SlateEngine/Engine/Graphics/Shader/ShaderCache.h>


void RenderableGeometry::OnInternalInit()
{

    SetBuffer(BuiltInMesh::CreateBox<VertexPNT>());

    m_constantBuffer = std::make_unique<DXConstantBuffer>();


    cbData.world = mat4x4();
    cbData.worldInverseTranspose = mat4x4();
    
    if (!connectedEntity->HasComponent<MaterialComponent>())
    {
        connectedEntity->AddComponent<MaterialComponent>();
    }
    
    m_material = &connectedEntity->GetComponent<MaterialComponent>();

    m_material->Set("$Ambient",  vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    m_material->Set("$Diffuse",  vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    m_material->Set("$Specular", vec4f(0.1f, 0.1f, 0.1f, 5.0f));

    //Create Vertex Shader 3D
    ShaderInformation vertexShaderInfo{};
    vertexShaderInfo.csoName = "Shaders\\TexturedLit\\Lit3DVS.cso";
    vertexShaderInfo.hlslFile = "Shaders\\TexturedLit\\Lit3DVS.hlsl";
    vertexShaderInfo.entryPoint = "main";

    m_vertexShader = ShaderCache::CreateVertexShader(vertexShaderInfo);
    m_vertexShader->CreateInputLayout(VertexPNT::inputLayout, ARRAYSIZE(VertexPNT::inputLayout));



    ShaderInformation pixelShaderInfo{};
    pixelShaderInfo.csoName = "Shaders\\TexturedLit\\Lit3DPS.cso";
    pixelShaderInfo.hlslFile = "Shaders\\TexturedLit\\Lit3DPS.hlsl";
    pixelShaderInfo.entryPoint = "main";

    //Create Pixel Shader 3D
    m_pixelShader = ShaderCache::CreatePixelShader(pixelShaderInfo);

    ConstantBufferDesc cbd{};
    cbd.cbSize = sizeof(ObjectConstantBuffer);
    m_constantBuffer->Create(cbd);
    m_constantBuffer->BindVS(BUFFER_ID::OBJECT_CONSTANT_BUFFER_ID);
    m_constantBuffer->BindPS(BUFFER_ID::OBJECT_CONSTANT_BUFFER_ID);

    m_vertexShader->Bind();
    m_pixelShader->Bind();

    buffers.emplace_back(m_vertexBuffer);
    buffers.emplace_back(m_indexBuffer);
    buffers.emplace_back(m_constantBuffer.get());

    SetCullMode((RasterizerState)0);
    //Calling Update once
    mat_ambient = m_material->GetPointer<vec4f>("$Ambient");
    mat_diff    = m_material->GetPointer<vec4f>("$Diffuse");
    mat_spec    = m_material->GetPointer<vec4f>("$Specular");
    OnUpdate(0);
}

void RenderableGeometry::SetTexture(DXTexture* texture)
{
    if (texture != nullptr) {
        texture->Bind(0);
        attachedTexture = texture;
    }
}


void RenderableGeometry::OnUpdate(float deltaTime)
{
    cbData.world = connectedEntity->GetComponent<Transform>().GetGlobal();
    cbData.worldInverseTranspose = connectedEntity->GetComponent<Transform>().GetGlobal().InverseTranspose();

    cbData.material.ambient = *mat_ambient;
    cbData.material.diffuse = *mat_diff;
    cbData.material.specular = *mat_spec;

    m_constantBuffer->MapAndUnMap(sizeof(ObjectConstantBuffer), &cbData);
}

void RenderableGeometry::SetCullMode(RasterizerState state, bool* ignoreState)
{
    cullMode = state;
    this->ignoreState = ignoreState;
}

void RenderableGeometry::OnRender(ID3D11DeviceContext* pDeviceContext)
{
    for (size_t i = 0; i < buffers.size(); i++)
    {
        buffers[i]->BindPipeline(BUFFER_ID::OBJECT_CONSTANT_BUFFER_ID);
    }

    m_vertexShader->Bind();
    m_vertexShader->UpdateInputLayout();
    m_pixelShader->Bind();

    attachedTexture->Bind();

    if (ignoreState != nullptr) {
        if (*ignoreState == false)DXRasterizerState::SetRasterizerState((RasterizerState)cullMode,DXApplication::Instance->GetDXContext().Get());
    }

    pDeviceContext->DrawIndexed(m_indices, 0u, 0u);

    attachedTexture->UnBind();
}

void RenderableGeometry::OnInit()
{
}

void RenderableGeometry::OnShutdown()
{
}
