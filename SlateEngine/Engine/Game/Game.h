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
#include <SlateEngine/Engine/Component/LuaScript.h>

#include <SlateEngine/Engine/Graphics/Shader/RenderPassManager.h>

class Game : public DXApplication
{
public:
    Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~Game();

    bool OnInit();
    void OnResize();
    void OnUpdateScene(float deltaTime);

    //Todo, Scene*
    void OnRenderScene();

    void UpdateGlobalConstantBuffers();

    void BeginClear();
    void PostClear();

    
    void SetGameState(GameState gs);
    GameState GetGameState() { return gameState; };

    static Game* Instance;

    bool renderWireframe = 0;

    static float clear[4];

    FrameConstantBuffer   FrameBufferConstantObject;
    LightConstantBuffer   LightConstantObject;

    // TEMPORARY
    DXTexture* m_crateTexture = nullptr;
    std::unique_ptr<DXTexture> m_grassTexture = nullptr;

    Entity* testEntity;
    Entity* testEntity2;

    //--

    Camera* m_camera{};


private:
    GameState gameState = GameState::NONE;

    std::unique_ptr<DXConstantBuffer> m_frameConstantBuffer = nullptr;
    std::unique_ptr<DXConstantBuffer> m_lightConstantBuffer = nullptr;

    std::shared_ptr<EntityManager> entityManager;
    std::shared_ptr<FileSystem> fileSystem;

};