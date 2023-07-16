#include <TestEngine/Engine/Game/Game.h>
#include <TestEngine/Engine/Vertex.h>

const D3D11_INPUT_ELEMENT_DESC VertexPC::inputLayout[2] = {
    { "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

Game::Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
    : DXApplication(hInstance, windowName, initWidth, initHeight)
{
}

Game::~Game()
{
}

bool Game::OnInit()
{
    if (!DXApplication::OnInit()) { return 0; }

    ComPtr<ID3DBlob> blob;
    //-- SHADER
    
    HR(CreateShaderFromFile(L"Shaders\\TestVS.cso", L"Shaders\\TestVS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_d3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf()));
    HR(m_d3dDevice->CreateInputLayout(VertexPC::inputLayout, ARRAYSIZE(VertexPC::inputLayout),
        blob->GetBufferPointer(), blob->GetBufferSize(), m_inputLayout.GetAddressOf()));
    HR(CreateShaderFromFile(L"Shaders\\TestPS.cso", L"Shaders\\TestPS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_d3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf()));

    //-----
    VertexPC vertices[] =
    {
        { XMFLOAT3(0.0f, 0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }
    };

    //Creating Vertex Buffer Descriptor
    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    //Creating Resource data for vertex buffer
    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertices;

    //Creating Vertex Buffer
    HR(m_d3dDevice->CreateBuffer(&bd, &sd, m_vertexBuffer.GetAddressOf()));

    //Set Input Vertex Data
    UINT stride = sizeof(VertexPC); 
    UINT offset = 0;                
    m_d3dContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

    m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_d3dContext->IASetInputLayout(m_inputLayout.Get());
    m_d3dContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_d3dContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    return true;
}

void Game::OnResize()
{
    DXApplication::OnResize();
}

void Game::OnUpdateScene(float dt)
{
}

void Game::OnRenderScene()
{
    static float rgba[4] = { 1.0f, 0.0f, 1.0f, 1.0f };

    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), rgba);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //Drawing triangle
    m_d3dContext->Draw(3, 0);
    HR(m_swapChain->Present(0, 0));
}