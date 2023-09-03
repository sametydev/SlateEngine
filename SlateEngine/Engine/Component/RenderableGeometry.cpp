#include <SlateEngine/Engine/Component/RenderableGeometry.h>

void RenderableGeometry::OnInternalInit()
{

    //Create our Vertex Buffer
    m_vertexBuffer               = new DXVertexBuffer();

    //Create our Index Buffer
    m_indexBuffer                = new DXIndexBuffer();

    SetBuffer(BuiltInMesh::CreateBox<VertexPNT>());


    m_objectConstantBuffer       = new DXConstantBuffer();

    cbData.world                 = mat4x4();
    cbData.worldInverseTranspose = mat4x4();

    cbData.material.ambient      = vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    cbData.material.diffuse      = vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    cbData.material.specular     = vec4f(0.1f, 0.1f, 0.1f, 5.0f);

    //Create Vertex Shader 3D
    m_vertexShader               = new DXVertexShader();
    m_vertexShader->Compile(L"Shaders\\TexturedLit\\Lit3DVS.cso", L"Shaders\\TexturedLit\\Lit3DVS.hlsl", "main");
    m_vertexShader->CreateInputLayout(VertexPNT::inputLayout, ARRAYSIZE(VertexPNT::inputLayout));

    //Create Pixel Shader 3D
    m_pixelShader                = new DXPixelShader();
    m_pixelShader->Compile(L"Shaders\\TexturedLit\\Lit3DPS.cso", L"Shaders\\TexturedLit\\Lit3DPS.hlsl", "main");

    ConstantBufferDesc cbd{};
    cbd.cbSize                   = sizeof(ObjectConstantBuffer);
    m_objectConstantBuffer->Create(cbd);

    m_objectConstantBuffer->BindVS(0);
    m_objectConstantBuffer->BindPS(0);

    m_vertexShader->Bind();
    m_pixelShader->Bind();

    SetCullMode((RasterizerState)0);
    //Calling Update once
    OnUpdate(0);
}

void RenderableGeometry::SetTexture(DXTexture* texture)
{
    texture->Bind(0);
    attachedTexture = texture;
}


void RenderableGeometry::OnUpdate(float deltaTime)
{
    cbData.world                 = connectedEntity->GetComponent<Transform>().GetGlobal();
    cbData.worldInverseTranspose = connectedEntity->GetComponent<Transform>().GetGlobal().InverseTranspose();

    m_objectConstantBuffer->Map(sizeof(ObjectConstantBuffer), &cbData);
    m_objectConstantBuffer->UnMap();
}

void RenderableGeometry::SetCullMode(RasterizerState state, bool* ignoreState)
{
    cullMode = state;
    this->ignoreState = ignoreState;
}

void RenderableGeometry::OnRender()
{
    m_vertexBuffer->BindPipeline(0);
    m_indexBuffer->BindPipeline(0);
    m_vertexShader->Bind();
    m_vertexShader->UpdateInputLayout();
    m_pixelShader->Bind();
    attachedTexture->Bind();
    m_objectConstantBuffer->BindVS(0);
    m_objectConstantBuffer->BindPS(0);

    if(*ignoreState == false)DXRasterizerState::Instance->SetRasterizerState((RasterizerState)cullMode);

    DXApplication::Instance->GetDXContext().Get()->DrawIndexed(m_indices, 0, 0);
}
