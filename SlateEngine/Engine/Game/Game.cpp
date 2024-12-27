#include <SlateEngine/Engine/Game/Game.h>
#include <SlateEngine/Engine/Input/InputSystem.h>
#include <SlateEngine/Engine/Physics/PhysicsFactory.h>
#include <SlateEngine/Engine/NativeScripting/ScriptRegistry.h>
#include <SlateEngine/Engine/Graphics/DXBasicBatch.h>
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
    delete script1;
    FreeLibrary(hModule);
}

bool Game::OnInit()
{
    if (!DXApplication::OnInit()) { return 0; }

    CreateCamera();

    enginePlayer->OnInit(hWindow, m_d3dDevice.Get(), m_d3dContext.Get());
    enginePlayer->ResizeViewport(m_clientW, m_clientH);

    CreateFileSystem();

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

        testEntity->AddComponent<MaterialComponent>();
        testEntity->GetComponent<MaterialComponent>().AddShader("Lit3DVS");
        testEntity->GetComponent<MaterialComponent>().AddShader("Lit3DPS");

        testEntity->AddComponent<RenderableGeometry>();


        testEntity->AddComponent<LuaScript>();
        testEntity->GetComponent<LuaScript>().SetScriptPath(PathMaker::Make(gDXApp->GetWorkingDir(), "Assets\\Scripts\\Test.lua").c_str());

        testEntity->GetComponent<Transform>().SetPosition({ 0.f,2.f,0.f });

        RenderableGeometry& r = testEntity->GetComponent<RenderableGeometry>();
        r.GetMaterial().AddTexture(m_crateTexture);

        testEntity2 = new Entity();

        entityManager->RegisterEntity(testEntity2, "Test Entity 2");
        testEntity2->AddComponent<MaterialComponent>();
        testEntity2->GetComponent<MaterialComponent>().AddShader("Lit3DVS");
        testEntity2->GetComponent<MaterialComponent>().AddShader("Lit3DPS");

        testEntity2->AddComponent<RenderableGeometry>();

        RenderableGeometry& r2 = testEntity2->GetComponent<RenderableGeometry>();
        //r2.SetCullMode(CULL_BACK, &renderWireframe);
        r2.GetMaterial().AddTexture(m_grassTexture.get());
        r2.GetTransform().SetScale({ 10.f, 0.2f, 10.f });
    }
    
    //Creating Constant Buffers;
    CreateGlobalConstantBuffers();

    // --- TEMPORARY CODE ---- //
    // NATIVE SCRIPTING CONCEPT!! NOT FINAL

    hModule = LoadLibrary(L"GamePlugin.dll");
    if (!hModule) {
        std::cerr << "Error on DLL load!" << std::endl;
        MessageBoxA(0,"Error on DLL load!",0,0);
        return 1;
    }



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


    // --- TEMPORARY CODE ---- //

    CreateGrid();

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

    UpdateGlobalConstantBuffers();

    if (renderWireframe)DXRasterizerState::SetRasterizerState(RasterizerState::CULL_WIREFRAME,GetDXContext());

    entityManager->OnUpdate(deltaTime,gameState);
}

void Game::OnRenderScene()
{
    entityManager->OnRender(GetDXContext());
    RenderGrid();
}

void Game::UpdateGlobalConstantBuffers()
{
    FrameBufferConstantObject.eyePos = m_camera->GetPos();
    FrameBufferConstantObject.view = m_camera->GetViewMatrix();
    FrameBufferConstantObject.proj = m_camera->GetProjectionMatrix();

    //Updating VS Cbuffer
    m_frameConstantBuffer->Map(sizeof(FrameConstantBuffer), &FrameBufferConstantObject);
    m_frameConstantBuffer->UnMap();

    //Updating PS Cbuffer
    m_lightConstantBuffer->Map(sizeof(LightConstantBuffer), &LightConstantObject);
    m_lightConstantBuffer->UnMap();
}

void Game::CreateCamera()
{
    m_camera = new Camera(65.f, GetAspectRatio(), 0.01f, 1000.0f);
    m_camera->SetPosition(vec3f(0, 10, 0));
}

void Game::CreateFileSystem()
{
    fileSystem = std::make_shared<FileSystem>();
    fileSystem->Init();
}

void Game::OnLateRender()
{
}

void Game::OnLateUpdate(float deltaTime)
{
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

void Game::CreateGrid()
{
    SetGridBuffer(BuiltInMesh::CreateGrid<VertexPC>(200.0f, 200.0f, 100, 100, vec4f(0.25f, 0.25f, 0.25f, 1.f)));

    m_gridConstantBuffer = std::make_unique<DXConstantBuffer>();

    gridConstantBufferData.world                    = mat4x4();
    gridConstantBufferData.worldInverseTranspose    = mat4x4();

    ConstantBufferDesc cbd{};
    cbd.cbSize = sizeof(ObjectConstantBuffer);
    m_gridConstantBuffer->Create(cbd);

    m_gridVS = ShaderCache::GetVertexShader("GridVS");
    m_gridPS = ShaderCache::GetPixelShader("GridPS");
}

void Game::RenderGrid()
{
    gridMatrix.SetIdentity();
    gridMatrix.translated({ 0.0f,0.0f,0.0f });

    gridConstantBufferData.world                 = gridMatrix;
    gridConstantBufferData.worldInverseTranspose = gridMatrix.InverseTranspose();

    m_gridConstantBuffer->MapAndUnMap(sizeof(ObjectConstantBuffer), &gridConstantBufferData);

    m_gridVS->Bind();
    m_gridPS->Bind();

    m_gridConstantBuffer->BindPipeline(0);
    m_gridIndexBuffer->BindPipeline(0);
    m_gridVertexBuffer->BindPipeline(0);
    DXRasterizerState::SetRasterizerState(RasterizerState::CULL_WIREFRAME, GetDXContext());

    GetDXContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    GetDXContext()->DrawIndexed(m_gridIndices, 0u, 0u);
}

void Game::SetGridBuffer(const MeshData<VertexPC, DWORD>& meshData)
{   
    //Reset old buffers
    if (m_gridVertexBuffer)m_gridVertexBuffer->Reset();
    if (m_gridIndexBuffer)m_gridIndexBuffer->Reset();

    //Creating Vertex Buffer
    VertexBufferDesc vbd{};
    vbd.cbSize = (UINT)meshData.vVertex.size() * sizeof(VertexPC);
    vbd.cbStride = sizeof(VertexPC);
    vbd.pData = meshData.vVertex.data();

    m_gridVertexBuffer = std::make_unique<DXVertexBuffer>();
    m_gridVertexBuffer->Create(vbd);

    //Storing indices count
    m_gridIndices = (UINT)meshData.vIndices.size();

    //Creating Index Buffer
    IndexBufferDesc ibd{};
    ibd.cbSize = m_gridIndices * sizeof(DWORD);
    ibd.pData = meshData.vIndices.data();

    m_gridIndexBuffer = std::make_unique<DXIndexBuffer>();
    m_gridIndexBuffer->Create(ibd);
}

void Game::CreateGlobalConstantBuffers()
{
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
}
