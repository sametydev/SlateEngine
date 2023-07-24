#pragma once
#include <TestEngine/Engine/Graphics/DXApplication.h>
#include <TestEngine/Engine/Editor/EditorUI.h>
#include <TestEngine/Engine/Component/PointLight.h>
#include <TestEngine/Engine/Component/SpotLight.h>
#include <TestEngine/Engine/Component/DirectionalLight.h>
#include <TestEngine/Engine/BuiltInMesh.h>
#include <TestEngine/Engine/Graphics/Vertex.h>
#include <TestEngine/Engine/Graphics/Buffer/DXBuffer.h>
#include <TestEngine/Engine/Graphics/Shader/DXVertexShader.h>
#include <TestEngine/Engine/Graphics/Shader/DXPixelShader.h>
#include <TestEngine/Engine/Graphics/Buffer/DXVertexBuffer.h>
#include <TestEngine/Engine/Graphics/Buffer/DXIndexBuffer.h>
#include <TestEngine/Engine/Graphics/Buffer/DXConstantBuffer.h>

class Game : public DXApplication
{
public:
    Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~Game();

    bool OnInit();
    void OnResize();
    void OnUpdateScene(float deltaTime);


    void SetMesh(const MeshData<VertexPNC>& meshData);

    //Todo, Scene*
    void OnRenderScene();


    //Temporary variables
    static Game* Instance;
    static float x;
    static float y;
    static float py;
    static float tx;
    static float scale;

    static float clear[4];

    bool renderWireframe = false;

    DirectionalLight m_directionalLight;
    PointLight m_pointLight;
    SpotLight m_spotLight;
    VS_ConstantBuffer   m_vsCBufferData;
    PS_ConstantBuffer   m_psCBufferData;


private:
    ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;
    DXVertexBuffer* m_vertexBuffer = nullptr;
    DXIndexBuffer* m_indexBuffer = nullptr;

    DXConstantBuffer* m_constantBufferVS = nullptr;
    DXConstantBuffer* m_constantBufferPS = nullptr;

    UINT m_indexCount;

    DXVertexShader* vertexShader = nullptr;
    DXPixelShader* pixelShader = nullptr;	

    ComPtr<ID3D11RasterizerState> m_wireFrameRasterizer = nullptr;

};