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
#include <SlateEngine/Engine/Component/Script.h>
#include <SlateEngine/Engine/Module/GameModule.h>
#include <SlateEngine/Engine/Graphics/Shader/RenderPassManager.h>

class ENGINE_API Game : public DXApplication
{
public:
    Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~Game();

    bool OnInit();
    void OnResize();
    void OnUpdateScene(float deltaTime, ID3D11DeviceContext* pContext);

    //Todo, Scene*
    void OnRenderScene(ID3D11DeviceContext* pContext);

    void OnLateRender();
    void OnLateUpdate(float deltaTime);
    
    void SetGameState(GameState gs);
    GameState GetGameState() { return gameState; };

    static Game* Instance;

    bool renderWireframe = 0;

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
    void CreateGlobalConstantBuffers();
    void UpdateGlobalConstantBuffers();

    void CreateCamera();
    void CreateFileSystem();


    GameState gameState = GameState::NONE;

    //-----------------------------------------------------------------------|


    //Global Buffers
    std::unique_ptr<DXConstantBuffer> m_frameConstantBuffer = nullptr;
    std::unique_ptr<DXConstantBuffer> m_lightConstantBuffer = nullptr;
    //-----------------------------------------------------------------------|


    //Managers
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<FileSystem> fileSystem;
    std::unique_ptr<GameModule> gameModule;
    std::unique_ptr<ScriptRegistry> scriptRegistry;
    //-----------------------------------------------------------------------|


    //C++ Native runtime scripting
    Script *script1 = nullptr;
    //-----------------------------------------------------------------------|
};