#include <TestEngine/Engine/Game/Game.h>
#include <TestEngine/Engine/Editor/Windows/LogWindow.h>
#include <TestEngine/Engine/Graphics/2D/D2DContext.h>

Game* Game::Instance = nullptr;

//TEMPORARY!!
float Game::x = 0.0f;
float Game::y = 0.0f;
float Game::py = 0.0f;
float Game::tx = 0.0f;
float Game::scale = 1.0f;


Game::Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
    : DXApplication(hInstance, windowName, initWidth, initHeight),
    m_vsCBufferData(),
    m_psCBufferData(),
    m_directionalLight(),
    m_pointLight(),
    m_spotLight()
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

    //Create Vertex Shader
    vertexShader = new DXVertexShader();
    vertexShader->Compile(L"Shaders\\LitVS.cso", L"Shaders\\LitVS.hlsl", "VS");
    vertexShader->CreateInputLayout(VertexPNC::inputLayout,ARRAYSIZE(VertexPNC::inputLayout));
    LogWindow::Instance->AddLog("[Debug] Compiled Vertex Shader\n");


    //Create Pixel Shader
    pixelShader = new DXPixelShader();
    pixelShader->Compile(L"Shaders\\LitPS.cso", L"Shaders\\LitPS.hlsl", "PS");
    LogWindow::Instance->AddLog("[Debug] Compiled Pixel Shader\n");


    //Create our Vertex Buffer
    m_vertexBuffer = new DXVertexBuffer();

    //Create our Index Buffer
    m_indexBuffer = new DXIndexBuffer();

    //Creating Mesh
    auto meshData = BuiltInMesh::CreateBox<VertexPNC>();
    SetMesh(meshData);


    //Creating Constant Buffers;
    m_constantBufferVS = new DXConstantBuffer();
    m_constantBufferPS = new DXConstantBuffer();


    ConstantBufferDesc cbdvs{};
    cbdvs.cbSize = sizeof(VS_ConstantBuffer);
    m_constantBufferVS->Create(cbdvs);

    ConstantBufferDesc cbdps{};
    cbdps.cbSize = sizeof(PS_ConstantBuffer);
    m_constantBufferPS->Create(cbdps);
 
    m_directionalLight.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    m_directionalLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    m_directionalLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_directionalLight.direction = XMFLOAT3(-0.577f, -0.577f, 0.577f);
  
    m_pointLight.position = XMFLOAT3(0.0f, 0.0f, -10.0f);
    m_pointLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
    m_pointLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
    m_pointLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_pointLight.attenutation = XMFLOAT3(0.0f, 0.1f, 0.0f);
    m_pointLight.range = 25.0f;

    m_spotLight.position = XMFLOAT3(0.0f, 0.0f, -5.0f);
    m_spotLight.direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
    m_spotLight.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    m_spotLight.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_spotLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_spotLight.attenutation = XMFLOAT3(1.0f, 0.0f, 0.0f);
    m_spotLight.spot = 12.0f;
    m_spotLight.range = 10000.0f;

    m_vsCBufferData.world = XMMatrixIdentity();
    m_vsCBufferData.view = XMMatrixTranspose(XMMatrixLookAtLH(
        XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    ));
    m_vsCBufferData.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, GetAspectRatio(), 1.0f, 1000.0f));
    m_vsCBufferData.worldInvTranspose = XMMatrixIdentity();


    m_psCBufferData.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_psCBufferData.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_psCBufferData.material.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 5.0f);

    m_psCBufferData.dirLight = m_directionalLight;

    m_psCBufferData.eyePos = XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f);

    m_constantBufferPS->Map(sizeof(PS_ConstantBuffer), &m_psCBufferData);
    m_constantBufferPS->UnMap();



    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthClipEnable = true;
    HR(m_d3dDevice->CreateRasterizerState(&rasterizerDesc, m_wireFrameRasterizer.GetAddressOf()));


    m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    vertexShader->Bind();
    m_constantBufferVS->BindVS(0);

    m_constantBufferPS->BindPS(1);
    pixelShader->Bind();



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
    m_vsCBufferData.world = XMMatrixTranspose(W);
    m_vsCBufferData.worldInvTranspose = XMMatrixTranspose(InverseTranspose(W));

    EditorUI::instance()->OnUpdate();

    //Updating VS Cbuffer
    m_constantBufferVS->Map(sizeof(VS_ConstantBuffer), &m_vsCBufferData);
    m_constantBufferVS->UnMap();

    //Updating PS Cbuffer
    m_constantBufferPS->Map(sizeof(PS_ConstantBuffer), &m_psCBufferData);
    m_constantBufferPS->UnMap();

    m_d3dContext->RSSetState(renderWireframe ? m_wireFrameRasterizer.Get() : nullptr);
}
void Game::SetMesh(const MeshData<VertexPNC>& meshData)
{
    //Reset old buffers
    m_vertexBuffer->Reset();
    m_indexBuffer->Reset();

    //Creating Vertex Buffer
    VertexBufferDesc vbd{};
    vbd.cbSize = (UINT)meshData.vVertex.size() * sizeof(VertexPNC);
    vbd.cbStride = sizeof(VertexPNC);
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