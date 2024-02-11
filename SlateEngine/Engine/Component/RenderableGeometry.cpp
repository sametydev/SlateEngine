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

    m_material->Set("$Ambient", vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    m_material->Set("$Diffuse", vec4f(1.0f, 1.0f, 1.0f, 1.0f));
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
    m_constantBuffer->BindVS(0);
    m_constantBuffer->BindPS(0);

    m_vertexShader->Bind();
    m_pixelShader->Bind();

    buffers.emplace_back(m_vertexBuffer);
    buffers.emplace_back(m_indexBuffer);
    buffers.emplace_back(m_constantBuffer.get());

    SetCullMode((RasterizerState)0);
    //Calling Update once
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

    cbData.material.ambient  = m_material->Get<vec4f>("$Ambient");
    cbData.material.diffuse  = m_material->Get<vec4f>("$Diffuse");
    cbData.material.specular = m_material->Get<vec4f>("$Specular");

    m_constantBuffer->MapAndUnMap(sizeof(ObjectConstantBuffer), &cbData);
}

void RenderableGeometry::SetCullMode(RasterizerState state, bool* ignoreState)
{
    cullMode = state;
    this->ignoreState = ignoreState;
}

void RenderableGeometry::OnRender()
{
    for (size_t i = 0; i < buffers.size(); i++)
    {
        buffers[i]->BindPipeline(0);
    }

    m_vertexShader->Bind();
    m_vertexShader->UpdateInputLayout();
    m_pixelShader->Bind();

    attachedTexture->Bind();

    if (ignoreState != nullptr) {
        if (*ignoreState == false)DXRasterizerState::SetRasterizerState((RasterizerState)cullMode,DXApplication::Instance->GetDXContext().Get());
    }

    DXApplication::Instance->GetDXContext().Get()->DrawIndexed(m_indices, 0u, 0u);

    attachedTexture->UnBind();
}

void RenderableGeometry::OnInit()
{
}

void RenderableGeometry::OnShutdown()
{
}
