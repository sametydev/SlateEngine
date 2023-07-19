#pragma once
#include <TestEngine/Engine/Graphics/DXApplication.h>
#include <TestEngine/Engine/Editor/EditorUI.h>

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

    //For Temporary
    struct ConstantBuffer
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX proj;
        DirectX::XMFLOAT4 color;
    };
    ConstantBuffer cbuffer;
    


    //Temporary variables
    static Game* Instance;
    static float x;
    static float y;
    static float py;
    static float tx;
    static float scale;

private:
    ComPtr<ID3D11InputLayout> m_inputLayout;
    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    ComPtr<ID3D11Buffer> m_constantBuffer;
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D10Blob>  m_blob;
};