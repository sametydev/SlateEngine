#pragma once
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include <SlateEngine/Engine/Editor/EditorUI.h>
#include <SlateEngine/Engine/Component/PointLight.h>
#include <SlateEngine/Engine/Component/SpotLight.h>
#include <SlateEngine/Engine/Component/DirectionalLight.h>
#include <SlateEngine/Engine/Graphics/Vertex.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXBuffer.h>
#include <SlateEngine/Engine/Graphics/Shader/DXVertexShader.h>
#include <SlateEngine/Engine/Graphics/Shader/DXPixelShader.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXVertexBuffer.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXIndexBuffer.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXConstantBuffer.h>
#include <SlateEngine/Engine/Graphics/BuiltInMesh.h>
#include <SlateEngine/Engine/Graphics/Texture/DXTexture.h>
#include <SlateEngine/Engine/Component/RenderableObject.h>
#include <SlateEngine/Engine/Component/Camera.h>


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


    //Temporary variables
    static Game* Instance;
    static float x;
    static float y;
    static float py;
    static float tx;
    static float scale;

    static float clear[4];

    bool renderWireframe = false;

    FrameConstantBuffer   FrameBufferConstantObject;
    LightConstantBuffer  LightConstantObject;

    DXTexture* m_crateTexture = nullptr;
private:
    //For Testing
    void InitializeLighting();
    ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;

    DXConstantBuffer* m_frameConstantBuffer = nullptr;
    DXConstantBuffer* m_lightConstantBuffer = nullptr;

    RenderableObject* m_box;
    Camera* m_camera{};
    ComPtr<ID3D11SamplerState> samplerState;				    

    DXVertexShader* vertexShader3D = nullptr;
    DXPixelShader* pixelShader3D = nullptr;

    ComPtr<ID3D11RasterizerState> m_wireFrameRasterizer = nullptr;


};