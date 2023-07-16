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

    VertexPC vertices[] =
    {
        { XMFLOAT3(0.0f, 0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }
    };

    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

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

    HR(m_swapChain->Present(0, 0));
}