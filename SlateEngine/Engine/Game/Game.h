#pragma once
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include <SlateEngine/Engine/Component/Components.h>
#include <SlateEngine/Engine/Graphics/Vertex.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXBuffer.h>
#include <SlateEngine/Engine/Graphics/Shader/DXVertexShader.h>
#include <SlateEngine/Engine/Graphics/Shader/DXPixelShader.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXVertexBuffer.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXIndexBuffer.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXConstantBuffer.h>
#include <SlateEngine/Engine/Graphics/BuiltInMesh.h>
#include <SlateEngine/Engine/Graphics/Texture/DXTexture.h>
#include <SlateEngine/Engine/Component/RenderableGeometry.h>
#include <SlateEngine/Engine/Component/Camera.h>
#include <SlateEngine/Engine/Entity/Entity.h>
#include <SlateEngine/Engine/Entity/EntityManager.h>
#include <SlateEngine/Engine/FileSystem/FileSystem.h>

enum GameState {
    NONE = 0,
    PLAYING,
    PAUSED
};

class Game : public DXApplication
{
public:
    Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~Game();

    bool OnInit();
    void OnResize();
    void OnUpdateScene(float deltaTime);


    void SetMesh(const MeshData<VertexPNT>& meshData);

    //Todo, Scene*
    void OnRenderScene();

    void UpdateGlobalConstantBuffers();

    void BeginClear();
    void PostClear();

    inline int GetClientW() { return m_clientW; }
    inline int GetClientH() { return m_clientH; }

    GameState gameState = GameState::NONE;

    static Game* Instance;

    //Temporary variables
    float py;
    float tx;

    bool renderWireframe = 0;
    //--

    static float clear[4];

    FrameConstantBuffer   FrameBufferConstantObject;
    LightConstantBuffer  LightConstantObject;

    DXTexture* m_crateTexture = nullptr;
    DXTexture* m_grassTexture = nullptr;

    Entity* testEntity;
    Entity* testEntity2;

    Camera* m_camera{};

private:
    DXConstantBuffer* m_frameConstantBuffer = nullptr;
    DXConstantBuffer* m_lightConstantBuffer = nullptr;

    EntityManager* entityManager;
    FileSystem* fileSystem;
};