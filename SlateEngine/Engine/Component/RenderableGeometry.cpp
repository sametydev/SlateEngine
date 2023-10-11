#include <SlateEngine/Engine/Component/RenderableGeometry.h>
#include <SlateEngine/Engine/Graphics/Shader/ShaderCache.h>


void RenderableGeometry::OnInternalInit()
{

    //Create our Vertex Buffer
    m_vertexBuffer = std::make_unique<DXVertexBuffer>();
    //Create our Index Buffer
    m_indexBuffer = std::make_unique<DXIndexBuffer>();

    SetBuffer(BuiltInMesh::CreateBox<VertexPNT>());

    m_objectConstantBuffer = std::make_unique<DXConstantBuffer>();

    cbData.world = mat4x4();
    cbData.worldInverseTranspose = mat4x4();
    m_material = new MaterialComponent();
    cbData.material.ambient = vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    cbData.material.diffuse = vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    cbData.material.specular = vec4f(0.1f, 0.1f, 0.1f, 5.0f);

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
    if (texture != nullptr) {
        texture->Bind(0);
        attachedTexture = texture;
    }

}


void RenderableGeometry::OnUpdate(float deltaTime)
{
    cbData.world = connectedEntity->GetComponent<Transform>().GetGlobal();
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

    if (ignoreState != nullptr) {
        if (*ignoreState == false)DXRasterizerState::SetRasterizerState((RasterizerState)cullMode,DXApplication::Instance->GetDXContext().Get());
    }

    DXApplication::Instance->GetDXContext().Get()->DrawIndexed(m_indices, 0u, 0u);

    attachedTexture->UnBind();
}