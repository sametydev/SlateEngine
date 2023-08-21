#include <SlateEngine/Engine/Game/Game.h>
#include <SlateEngine/Engine/Editor/Windows/LogWindow.h>
#include <SlateEngine/Engine/Input/InputSystem.h>
#include <SlateEngine/Engine/Editor/EditorUI.h>

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

    if(!IS_COOKED)EditorUI::instance()->ResizeViewport(m_clientW, m_clientH);

    LogWindow::Instance->AddLog("[Info] DirectX 11 Initialized!\n");
    LogWindow::Instance->AddLog("[Info] Game OnInit\n");


    m_camera = new Camera(45.f, GetAspectRatio(), 0.01f, 1000.0f);
    m_camera->SetPosition(vec3f(0,0,-10));


    entityManager = new EntityManager();


    D3D11_SAMPLER_DESC sampDesc{};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    sampDesc.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;
    HR(m_d3dDevice->CreateSamplerState(&sampDesc, samplerState.GetAddressOf()));

    m_crateTexture = new DXTexture();
    m_crateTexture->Load(L"Textures\\Crate.dds");

    testEntity = new Entity();

    entityManager->RegisterEntity(testEntity,"Test Object");
    testEntity->AddComponent<RenderableObject>();

    RenderableObject& r = testEntity->GetComponent<RenderableObject>();
    r.SetTexture(m_crateTexture);

    Entity* entttt = new Entity();
    entityManager->RegisterEntity(entttt, "Test Empty Entity");

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


    m_frameConstantBuffer->BindVS(1);

    m_frameConstantBuffer->BindPS(1);
    m_lightConstantBuffer->BindPS(2);

    m_d3dContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
    

    return true;
}

void Game::OnResize()
{
    DXApplication::OnResize();
    EditorUI::instance()->ResizeViewport(m_clientW, m_clientH);

    if (m_camera != nullptr)
    {
        FrameBufferConstantObject.proj = m_camera->GetProjectionMatrix();
    }
}

void Game::OnUpdateScene(float deltaTime)
{
    m_camera->Update(deltaTime);
#pragma region EDITOR_STUFF
    if (!IS_COOKED)EditorUI::instance()->OnUpdate();
#pragma endregion

#pragma region CAMERA_MOVEMENT
    constexpr float movementSpeed = 4.2f;
    constexpr float mouseSpeed = 0.1333f;

    if (InputSystem::IsKeyDown(Key::RMB)) {
        vec2f delta = InputSystem::delta;
        m_camera->mRot.y += delta.x * mouseSpeed;
        m_camera->mRot.x += delta.y * mouseSpeed;
    }

    if (InputSystem::IsKeyDown(Key::W)) {
        m_camera->mPos += m_camera->mForward * movementSpeed * deltaTime;
    }
    if (InputSystem::IsKeyDown(Key::S)) {
        m_camera->mPos -= m_camera->mForward * movementSpeed * deltaTime;
    }
    if (InputSystem::IsKeyDown(Key::D)) {
        m_camera->mPos += m_camera->mRight * movementSpeed * deltaTime;
    }
    if (InputSystem::IsKeyDown(Key::A)) {
        m_camera->mPos -= m_camera->mRight * movementSpeed * deltaTime;
    }
#pragma endregion

    FrameBufferConstantObject.eyePos = m_camera->GetPos();
    FrameBufferConstantObject.view = m_camera->GetViewMatrix();
    FrameBufferConstantObject.proj = m_camera->GetProjectionMatrix();

    py += 10.f * deltaTime, tx += 10.f * deltaTime;

    testEntity->GetComponent<RenderableObject>().GetTransform().SetRotation({py,tx,0});

    entityManager->OnUpdate(deltaTime);

    UpdateGlobalConstantBuffers();

    m_d3dContext->RSSetState(renderWireframe ? m_wireFrameRasterizer.Get() : nullptr);
}

float Game::clear[4] = {0.3f, 0.3f, 0.3f, 1.0f};

void Game::OnRenderScene()
{
    if (IS_COOKED) {
        ClearRenderTarget(clear);
    }
    else
    {
        EditorUI::instance()->ClearViewport(clear);
    }

    entityManager->OnRender();
    
    if (!IS_COOKED) {
        ClearRenderTarget(clear);
        EditorUI::instance()->OnRender();
    }
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
