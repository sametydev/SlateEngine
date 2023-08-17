#include <SlateEngine/Engine/Game/Game.h>
#include <SlateEngine/Engine/Editor/Windows/LogWindow.h>
#include <SlateEngine/Engine/Graphics/2D/D2DContext.h>


Game* Game::Instance = nullptr;

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
    InitializeLighting();

    m_camera = new Camera(45.f, GetAspectRatio(), 0.01f, 1000.0f);
    m_camera->SetPosition(vec3f(0,0,-10));

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

    m_box = new RenderableObject();
    m_box->SetTexture(m_crateTexture);
    m_box->SetBuffer(BuiltInMesh::CreateBox<VertexPNT>());


    //Creating Constant Buffers;
    m_frameConstantBuffer = new DXConstantBuffer();
    m_lightConstantBuffer = new DXConstantBuffer();

    ConstantBufferDesc cbd{};

    cbd.cbSize = sizeof(FrameConstantBuffer);
    m_frameConstantBuffer->Create(cbd);

    cbd.cbSize = sizeof(LightConstantBuffer);
    m_lightConstantBuffer->Create(cbd);

    m_lightConstantBuffer->Map(sizeof(LightConstantBuffer), &LightConstantObject);
    m_lightConstantBuffer->UnMap();


    D3D11_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthClipEnable = true;
    HR(m_d3dDevice->CreateRasterizerState(&rasterizerDesc, m_wireFrameRasterizer.GetAddressOf()));


    m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    vertexShader3D->Bind();

    m_frameConstantBuffer->BindVS(1);

    m_frameConstantBuffer->BindPS(1);
    m_lightConstantBuffer->BindPS(2);

    m_d3dContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
    pixelShader3D->Bind();

    return true;
}

void Game::OnResize()
{
    D2DContext::Instance->BeginResize();
    DXApplication::OnResize();
    D2DContext::Instance->OnResize();

    if (m_camera != nullptr)
    {
        FrameBufferConstantObject.proj = m_camera->GetProjectionMatrix();
    }
}

void Game::OnUpdateScene(float deltaTime)
{
    m_camera->Update(deltaTime);
    EditorUI::instance()->OnUpdate();

    FrameBufferConstantObject.eyePos = m_camera->GetPos();
    FrameBufferConstantObject.view = m_camera->GetViewMatrix();
    FrameBufferConstantObject.proj = m_camera->GetProjectionMatrix();

    m_box->OnUpdate(deltaTime);

    UpdateGlobalConstantBuffers();

    m_d3dContext->RSSetState(renderWireframe ? m_wireFrameRasterizer.Get() : nullptr);
}

float Game::clear[4] = {0.3f, 0.3f, 0.3f, 1.0f};

void Game::OnRenderScene()
{
    ClearRenderTarget(clear);

    m_box->OnRender();

    D2DContext::Instance->OnRender();
    EditorUI::instance()->OnRender();
    HR(m_swapChain->Present(0, 0));
}

void Game::UpdateGlobalConstantBuffers()
{
    //Updating VS Cbuffer
    m_frameConstantBuffer->Map(sizeof(FrameConstantBuffer), &FrameBufferConstantObject);
    m_frameConstantBuffer->UnMap();

    //Updating PS Cbuffer
    m_lightConstantBuffer->Map(sizeof(LightConstantBuffer), &LightConstantObject);
    m_lightConstantBuffer->UnMap();
}

void Game::InitializeLighting()
{
    LightConstantObject.pointLight[0].position = vec3f(0.0f, 0.0f, -10.0f);
    LightConstantObject.pointLight[0].ambient = vec4f(0.3f, 0.3f, 0.3f, 1.0f);
    LightConstantObject.pointLight[0].diffuse = vec4f(0.7f, 0.7f, 0.7f, 1.0f);
    LightConstantObject.pointLight[0].specular = vec4f(0.5f, 0.5f, 0.5f, 1.0f);
    LightConstantObject.pointLight[0].attenutation = vec3f(0.0f, 0.1f, 0.0f);
    LightConstantObject.pointLight[0].range = 25.0f;

    LightConstantObject.numDirLight = 0;
    LightConstantObject.numPointLight = 1;
    LightConstantObject.numSpotLight = 0;

    LightConstantObject.material.ambient = vec4f(0.5f, 0.5f, 0.5f, 1.0f);
    LightConstantObject.material.diffuse = vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    LightConstantObject.material.specular = vec4f(0.1f, 0.1f, 0.1f, 5.0f);

    LightConstantObject.dirLight[0].ambient = vec4f(0.2f, 0.2f, 0.2f, 1.0f);
    LightConstantObject.dirLight[0].diffuse = vec4f(0.8f, 0.8f, 0.8f, 1.0f);
    LightConstantObject.dirLight[0].specular = vec4f(0.5f, 0.5f, 0.5f, 1.0f);

    LightConstantObject.spotLight[0].position = vec3f(0.0f, 0.0f, -5.0f);
    LightConstantObject.spotLight[0].direction = vec3f(0.0f, 0.0f, 1.0f);
    LightConstantObject.spotLight[0].ambient = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
    LightConstantObject.spotLight[0].diffuse = vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    LightConstantObject.spotLight[0].specular = vec4f(1.0f, 1.0f, 1.0f, 1.0f);
    LightConstantObject.spotLight[0].attenutation = vec3f(1.0f, 0.0f, 0.0f);
    LightConstantObject.spotLight[0].spot = 12.0f;
    LightConstantObject.spotLight[0].range = 10000.0f;
}
