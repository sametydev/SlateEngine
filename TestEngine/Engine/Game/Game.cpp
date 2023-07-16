#include <TestEngine/Engine/Game/Game.h>

Game::Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
    : DXApplication(hInstance, windowName, initWidth, initHeight)
{
}

Game::~Game()
{
}

bool Game::OnInit()
{
    return DXApplication::OnInit();
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