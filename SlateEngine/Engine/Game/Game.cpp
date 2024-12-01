#include <SlateEngine/Engine/Game/Game.h>
#include <SlateEngine/Engine/Input/InputSystem.h>
#include <SlateEngine/Engine/Physics/PhysicsFactory.h>
#include <SlateEngine/Engine/NativeScripting/ScriptRegistry.h>
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
    m_camera = new Camera(65.f, GetAspectRatio(), 0.01f, 1000.0f);
    m_camera->SetPosition(vec3f(0, 0, -10));


    enginePlayer->OnInit(hWindow, m_d3dDevice.Get(), m_d3dContext.Get());
    enginePlayer->ResizeViewport(m_clientW, m_clientH);
    


    fileSystem = std::make_shared<FileSystem>();
    fileSystem->Init();

    entityManager = std::make_shared<EntityManager>();
    PhysicsFactory* pfactory = new PhysicsFactory();
    pfactory->Init();

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
        testEntity->GetComponent<LuaScript>().SetScriptPath(PathMaker::Make(gDXApp->GetWorkingDir(), "Assets\\Scripts\\Test.lua").c_str());

        testEntity->GetComponent<Transform>().SetPosition({ 0.f,2.f,0.f });

        RenderableGeometry& r = testEntity->GetComponent<RenderableGeometry>();
        //r.SetCullMode(CULL_BACK, &renderWireframe);
        r.GetMaterial().AddTexture(m_crateTexture);

        testEntity2 = new Entity();

        entityManager->RegisterEntity(testEntity2, "Test Entity 2");
        testEntity2->AddComponent<RenderableGeometry>();

        RenderableGeometry& r2 = testEntity2->GetComponent<RenderableGeometry>();
        //r2.SetCullMode(CULL_BACK, &renderWireframe);
        r2.GetMaterial().AddTexture(m_grassTexture.get());
        r2.GetTransform().SetScale({ 10.f, 0.2f, 10.f });
    }

    //Creating Constant Buffers;
    m_frameConstantBuffer = std::make_unique<DXConstantBuffer>();
    m_lightConstantBuffer = std::make_unique<DXConstantBuffer>();

    ConstantBufferDesc cbd{};
    cbd.cbSize = sizeof(FrameConstantBuffer);
    m_frameConstantBuffer->Create(cbd);

    cbd.cbSize = sizeof(LightConstantBuffer);
    m_lightConstantBuffer->Create(cbd);

    m_frameConstantBuffer->BindVS(BUFFER_ID::FRAME_CONSTANT_BUFFER_ID);

    m_frameConstantBuffer->BindPS(BUFFER_ID::FRAME_CONSTANT_BUFFER_ID);
    m_lightConstantBuffer->BindPS(BUFFER_ID::LIGHT_CONSTANT_BUFFER_ID);


    HMODULE hModule = LoadLibrary(L"GamePlugin.dll");
    if (!hModule) {
        std::cerr << "Error on DLL load!" << std::endl;
        MessageBoxA(0,"Error on DLL load!",0,0);
        return 1;
    }

    // --- TEMPORARY CODE ---- //
    // NATIVE SCRIPTING CONCEPT!! NOT FINAL

    new ScriptRegistry();

    std::string scriptToCreate = "MyTestScript";
    //using string for checking object eol

    script1 = ScriptRegistry::Instance->Create(scriptToCreate.c_str());

    Entity* dummy = new Entity();
    entityManager->RegisterEntity(dummy, "MyTestScript Entity!");
    script1->SetEntity(dummy);
    
    if (script1) {
        script1->OnInit();
    }

    //delete script1;

    //FreeLibrary(hModule);
    // --- TEMPORARY CODE ---- //

    return true;
}

void Game::OnResize()
{
    DXApplication::OnResize();

    enginePlayer->ResizeViewport(m_clientW, m_clientH);

    if (m_camera != nullptr)
    {
        m_camera->SetAspectRatio(GetAspectRatio());
        FrameBufferConstantObject.proj = m_camera->GetProjectionMatrix();
    }
}

void Game::OnUpdateScene(float deltaTime)
{
    m_camera->Update(deltaTime);

    enginePlayer->OnUpdate(deltaTime);


    FrameBufferConstantObject.eyePos = m_camera->GetPos();
    FrameBufferConstantObject.view   = m_camera->GetViewMatrix();
    FrameBufferConstantObject.proj   = m_camera->GetProjectionMatrix();

    UpdateGlobalConstantBuffers();

    if (renderWireframe)DXRasterizerState::SetRasterizerState(RasterizerState::CULL_WIREFRAME,GetDXContext());

    entityManager->OnUpdate(deltaTime,gameState);
}

float Game::clear[4] = {0.05f, 0.05f, 0.05f, 1.0f};

void Game::OnRenderScene()
{
    BeginClear();

    entityManager->OnRender(GetDXContext());

    PostClear();
    SwapChainPresent();
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
    //IS_COOKED ? ClearRenderTarget(clear) : editorSystem->ClearViewport(clear);

    enginePlayer->ClearViewport(clear);

}

void Game::PostClear()
{
    enginePlayer->OnRender(clear);
}

void Game::SetGameState(GameState gs)
{
    switch (gs)
    {
        case NONE:
            gameState = NONE;
            break;
        case PLAYING:
            if(gameState!=2)entityManager->SendSignalToComponents(ECSignalCommand::ON_INIT);
            break;
        case PAUSED:
            break;
    }
    gameState = gs;

}
