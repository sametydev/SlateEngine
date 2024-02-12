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

    DXRasterizerState::Initialize(GetDXDevice().Get(),GetDXContext().Get(),bEnableMsaa);
    gDXContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    if (!IS_COOKED)editorSystem->ResizeViewport(m_clientW, m_clientH);


    fileSystem = new FileSystem();
    fileSystem->Init();

    m_camera = new Camera(65.f, GetAspectRatio(), 0.01f, 1000.0f);
    m_camera->SetPosition(vec3f(0, 0, -10));

    entityManager = new EntityManager();

    //Temporary Game Scope
    {
        m_crateTexture = new DXTexture();
        m_crateTexture->Load("Assets\\Textures\\Crate.dds", TextureLoaderType::DDS);

        m_grassTexture = std::make_unique<DXTexture>();
        m_grassTexture->Load("Assets\\Textures\\Grass.jpg", TextureLoaderType::WIC);

        testEntity = new Entity();

        entityManager->RegisterEntity(testEntity, "Test Entity 1");
        testEntity->AddComponent<RenderableGeometry>();
        testEntity->AddComponent<LuaScript>();
        testEntity->GetComponent<LuaScript>().LoadScript("Assets\\Scripts\\Test.lua");

        testEntity->GetComponent<Transform>().SetPosition({ 0.f,2.f,0.f });

        RenderableGeometry& r = testEntity->GetComponent<RenderableGeometry>();
        r.SetCullMode(CULL_BACK, &renderWireframe);
        r.SetTexture(m_crateTexture);


        testEntity2 = new Entity();

        entityManager->RegisterEntity(testEntity2, "Test Entity 2");
        testEntity2->AddComponent<RenderableGeometry>();

        RenderableGeometry& r2 = testEntity2->GetComponent<RenderableGeometry>();
        r2.SetCullMode(CULL_BACK, &renderWireframe);
        r2.SetTexture(m_grassTexture.get());
        r2.GetTransform().SetScale({ 10.f, 0.2f, 10.f });
    }

    //Creating Constant Buffers;
    m_frameConstantBuffer = new DXConstantBuffer();
    m_lightConstantBuffer = new DXConstantBuffer();

    ConstantBufferDesc cbd{};
    cbd.cbSize = sizeof(FrameConstantBuffer);
    m_frameConstantBuffer->Create(cbd);

    cbd.cbSize = sizeof(LightConstantBuffer);
    m_lightConstantBuffer->Create(cbd);

    m_frameConstantBuffer->BindVS(1);

    m_frameConstantBuffer->BindPS(1);
    m_lightConstantBuffer->BindPS(2);

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

    if (renderWireframe)DXRasterizerState::SetRasterizerState(RasterizerState::CULL_WIREFRAME,GetDXContext().Get());

    entityManager->OnUpdate(deltaTime,gameState);
}

float Game::clear[4] = {0.2f, 0.2f, 0.2f, 1.0f};

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
    IS_COOKED ? ClearRenderTarget(clear) : editorSystem->ClearViewport(clear);
}

void Game::PostClear()
{
    if (!IS_COOKED) {
        ClearRenderTarget(clear);
        editorSystem->OnRender();
    }
}

void Game::SetGameState(GameState gs)
{
    gameState = gs;

    switch (gs)
    {
    case NONE:
        break;
    case PLAYING:
        entityManager->SendSignalToComponents(ECSignalCommand::ON_INIT);
        break;
    case PAUSED:
        break;
    }
}
