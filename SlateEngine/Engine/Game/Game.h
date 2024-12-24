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

#include <SlateEngine/Engine/Graphics/Shader/RenderPassManager.h>

class ENGINE_API Game : public DXApplication
{
public:
    Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~Game();

    bool OnInit();
    void OnResize();
    void OnUpdateScene(float deltaTime);

    //Todo, Scene*
    void OnRenderScene();

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
    std::shared_ptr<EntityManager> entityManager;
    std::shared_ptr<FileSystem> fileSystem;
    //-----------------------------------------------------------------------|


    //C++ Native runtime scripting
    Script *script1 = nullptr;
    HMODULE hModule = 0;
    //-----------------------------------------------------------------------|

    //Grid
    void CreateGrid();
    void RenderGrid();
    void SetGridBuffer(const MeshData<VertexPC, DWORD>& meshData);
    UINT m_gridIndices = 0;
    std::unique_ptr<DXVertexBuffer> m_gridVertexBuffer;
    std::unique_ptr<DXIndexBuffer> m_gridIndexBuffer;
    DXVertexShader* m_gridVS;
    DXPixelShader* m_gridPS;
    std::unique_ptr<DXConstantBuffer> m_gridConstantBuffer;
    ObjectConstantBuffer   gridConstantBufferData{};
    //-----------------------------------------------------------------------|
};