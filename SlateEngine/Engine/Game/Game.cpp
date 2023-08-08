#include <SlateEngine/Engine/Game/Game.h>
#include <SlateEngine/Engine/Editor/Windows/LogWindow.h>
#include <SlateEngine/Engine/Graphics/2D/D2DContext.h>


Game* Game::Instance = nullptr;

//TEMPORARY!!
float Game::x = 0.0f;
float Game::y = 0.0f;
float Game::py = 0.0f;
float Game::tx = 0.0f;
float Game::scale = 1.0f;


Game::Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
    : DXApplication(hInstance, windowName, initWidth, initHeight)
{
    if (!Instance)
    {
        Instance = this;
    }
}

Game::~Game()
{
}


bool Game::OnInit()
{
    if (!DXApplication::OnInit()) { return 0; }
    LogWindow::Instance->AddLog("[Info] DirectX 11 Initialized!\n");
    LogWindow::Instance->AddLog("[Info] Game OnInit\n");
    LogWindow::Instance->AddLog("[Info] 2D UI System OnInit\n");

    m_crateTexture = new DXTexture();
    m_crateTexture->Load(L"Textures\\Crate.dds");

    D3D11_SAMPLER_DESC sampDesc{};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    HR(m_d3dDevice->CreateSamplerState(&sampDesc, samplerState.GetAddressOf()));

    //Create Vertex Shader 3D
    vertexShader3D = new DXVertexShader();
    vertexShader3D->Compile(L"Shaders\\TexturedLit\\Lit3DVS.cso", L"Shaders\\TexturedLit\\Lit3DVS.hlsl", "main");
    vertexShader3D->CreateInputLayout(VertexPNT::inputLayout, ARRAYSIZE(VertexPNT::inputLayout));
    LogWindow::Instance->AddLog("[Debug] Compiled Vertex Shader\n");

    //Create Pixel Shader 3D
    pixelShader3D = new DXPixelShader();
    pixelShader3D->Compile(L"Shaders\\TexturedLit\\Lit3DPS.cso", L"Shaders\\TexturedLit\\Lit3DPS.hlsl", "main");
    LogWindow::Instance->AddLog("[Debug] Compiled Pixel Shader\n");

    //Create our Vertex Buffer
    m_vertexBuffer = new DXVertexBuffer();

    //Create our Index Buffer
    m_indexBuffer = new DXIndexBuffer();

    //Creating Mesh
    auto meshData = BuiltInMesh::CreateBox<VertexPNT>();
    SetMesh(meshData);


    //Creating Constant Buffers;
    m_frameBuffer = new DXConstantBuffer();
    m_renderBuffer = new DXConstantBuffer();
    m_resizeBuffer = new DXConstantBuffer();
    m_lightBuffer = new DXConstantBuffer();

    ConstantBufferDesc cbd{};

    cbd.cbSize = sizeof(OnFrameConstantBuffer);
    m_frameBuffer->Create(cbd);

    cbd.cbSize = sizeof(OnRenderConstantBuffer);
    m_renderBuffer->Create(cbd);

    cbd.cbSize = sizeof(OnResizeConstantBuffer);
    m_resizeBuffer->Create(cbd);

    cbd.cbSize = sizeof(LightConstantBuffer);
    m_lightBuffer->Create(cbd);
 
    InitializeLighting();

    m_lightBuffer->Map(sizeof(LightConstantBuffer), &LightObject);
    m_lightBuffer->UnMap();


    D3D11_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthClipEnable = true;
    HR(m_d3dDevice->CreateRasterizerState(&rasterizerDesc, m_wireFrameRasterizer.GetAddressOf()));


    m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    vertexShader3D->Bind();

    m_renderBuffer->BindVS(0);
    m_frameBuffer->BindVS(1);
    m_resizeBuffer->BindVS(2);

    m_renderBuffer->BindPS(0);
    m_frameBuffer->BindPS(1);
    m_resizeBuffer->BindPS(2);
    m_lightBuffer->BindPS(3);
    m_d3dContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
    m_crateTexture->Bind();
    pixelShader3D->Bind();

    return true;
}

void Game::OnResize()
{
    D2DContext::Instance->BeginResize();
    DXApplication::OnResize();
    D2DContext::Instance->OnResize();
}

void Game::OnUpdateScene(float deltaTime)
{

    py += 0.17f * deltaTime, tx += 0.27f * deltaTime;
    py = XMScalarModAngle(py);
    tx = XMScalarModAngle(tx);

    XMMATRIX W = XMMatrixRotationX(py) * XMMatrixRotationY(tx);
    OnRenderObject.world = XMMatrixTranspose(W);
    OnRenderObject.worldInverseTranspose = XMMatrixTranspose(InverseTranspose(W));

    EditorUI::instance()->OnUpdate();

    //Updating VS Cbuffer
    m_renderBuffer->Map(sizeof(OnRenderConstantBuffer), &OnRenderObject);
    m_renderBuffer->UnMap();

    m_frameBuffer->Map(sizeof(OnFrameConstantBuffer), &FrameBufferObject);
    m_frameBuffer->UnMap();

    m_resizeBuffer->Map(sizeof(OnResizeConstantBuffer), &OnResizeObject);
    m_resizeBuffer->UnMap();

    //Updating PS Cbuffer
    m_lightBuffer->Map(sizeof(LightConstantBuffer), &LightObject);
    m_lightBuffer->UnMap();

    m_d3dContext->RSSetState(renderWireframe ? m_wireFrameRasterizer.Get() : nullptr);
}
void Game::SetMesh(const MeshData<VertexPNT>& meshData)
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
    m_indexCount = (UINT)meshData.vIndices.size();

    //Creating Index Buffer
    IndexBufferDesc ibd{};
    ibd.cbSize = m_indexCount * sizeof(DWORD);
    ibd.pData = meshData.vIndices.data();
    m_indexBuffer->Create(ibd);
    m_indexBuffer->BindPipeline(0);

}

float Game::clear[4] = {0.3f, 0.3f, 0.3f, 1.0f};

void Game::OnRenderScene()
{
    ClearRenderTarget(clear);
    m_d3dContext->DrawIndexed(m_indexCount, 0, 0);

    D2DContext::Instance->OnRender();

    EditorUI::instance()->OnRender();
    HR(m_swapChain->Present(0, 0));
}

void Game::InitializeLighting()
{
    OnRenderObject.world = XMMatrixIdentity();
    FrameBufferObject.view = XMMatrixTranspose(XMMatrixLookAtLH(
        XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    ));
    OnResizeObject.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, GetAspectRatio(), 1.0f, 1000.0f));
    OnRenderObject.worldInverseTranspose = XMMatrixIdentity();

    LightObject.pointLight[0].position = XMFLOAT3(0.0f, 0.0f, -10.0f);
    LightObject.pointLight[0].ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
    LightObject.pointLight[0].diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
    LightObject.pointLight[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    LightObject.pointLight[0].attenutation = XMFLOAT3(0.0f, 0.1f, 0.0f);
    LightObject.pointLight[0].range = 25.0f;
    LightObject.numDirLight = 0;
    LightObject.numPointLight = 1;
    LightObject.numSpotLight = 0;
    FrameBufferObject.eyePos = XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f);
    LightObject.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    LightObject.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    LightObject.material.specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 5.0f);
    FrameBufferObject.eyePos = XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f);
}
