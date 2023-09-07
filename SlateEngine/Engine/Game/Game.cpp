#include <SlateEngine/Engine/Game/Game.h>
#include <SlateEngine/Engine/Input/InputSystem.h>
#include <ImGuizmo.h>
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

    if(!IS_COOKED)editorSystem->ResizeViewport(m_clientW, m_clientH);

    //LogWindow::Instance->AddLog("[Info] DirectX 11 Initialized!\n");
    //LogWindow::Instance->AddLog("[Info] Game OnInit\n");


    m_camera = new Camera(65.f, GetAspectRatio(), 0.01f, 1000.0f);
    m_camera->SetPosition(vec3f(0,0,-10));


    entityManager = new EntityManager();


    m_crateTexture = new DXTexture();
    m_crateTexture->Load(L"TestProject\\Textures\\Crate.dds",TextureLoaderType::DDS);
    m_grassTexture = new DXTexture();
    m_grassTexture->Load(L"TestProject\\Textures\\Grass.jpg", TextureLoaderType::WIC);

    testEntity = new Entity();

    entityManager->RegisterEntity(testEntity,"Test Entity 1");
    testEntity->AddComponent<RenderableGeometry>();
    testEntity->GetComponent<Transform>().SetPosition({0.f,2.f,0.f});

    RenderableGeometry& r = testEntity->GetComponent<RenderableGeometry>();
    r.SetCullMode(CULL_BACK,&renderWireframe);
    r.SetTexture(m_crateTexture);

    testEntity2 = new Entity();

    entityManager->RegisterEntity(testEntity2, "Test Entity 2");
    testEntity2->AddComponent<RenderableGeometry>();

    RenderableGeometry& r2 = testEntity2->GetComponent<RenderableGeometry>();
    r2.SetCullMode(CULL_BACK, &renderWireframe);
    r2.SetTexture(m_grassTexture);

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



    m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    m_frameConstantBuffer->BindVS(1);

    m_frameConstantBuffer->BindPS(1);
    m_lightConstantBuffer->BindPS(2);

    //m_d3dContext->PSSetSamplers(0, 1, DXRasterizerState::Instance->SSWrap.GetAddressOf());

    return true;
}

void Game::OnResize()
{
    DXApplication::OnResize();
    if(editorSystem)editorSystem->ResizeViewport(m_clientW, m_clientH);

    if (m_camera != nullptr)
    {
        m_camera->SetAspectRatio(GetAspectRatio());
        FrameBufferConstantObject.proj = m_camera->GetProjectionMatrix();
    }
}

void Game::OnUpdateScene(float deltaTime)
{
    m_camera->Update(deltaTime);
#pragma region EDITOR_STUFF
    if (!IS_COOKED) { editorSystem->OnUpdate(deltaTime);}
#pragma endregion

    FrameBufferConstantObject.eyePos = m_camera->GetPos();
    FrameBufferConstantObject.view   = m_camera->GetViewMatrix();
    FrameBufferConstantObject.proj   = m_camera->GetProjectionMatrix();
    UpdateGlobalConstantBuffers();

    if (gameState == (GameState::PLAYING)) {

        py += 10.f * deltaTime, tx += 10.f * deltaTime;
        testEntity->GetComponent<RenderableGeometry>().GetTransform().SetRotation({ py,tx,0 });

    }

    if (renderWireframe)GetRasterizerState().SetRasterizerState(RasterizerState::CULL_WIREFRAME);


    entityManager->OnUpdate(deltaTime,gameState);
}

float Game::clear[4] = {0.3f, 0.3f, 0.3f, 1.0f};

void Game::OnRenderScene()
{
    BeginClear();

    entityManager->OnRender();


    PostClear();

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

void Game::BeginClear()
{
    if (IS_COOKED) {
        ClearRenderTarget(clear);
    }
    else
    {
        editorSystem->ClearViewport(clear);
    }
}

void Game::PostClear()
{
    if (!IS_COOKED) {
        ClearRenderTarget(clear);
        editorSystem->OnRender();
    }
}
