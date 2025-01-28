#include "Player.h"


Player* Player::Instance = nullptr;



Player::Player()
{
	game = Game::Instance;
	if (!Instance)
	{
		Instance = this;
	}
}

Player::~Player()
{
}

void Player::OnInit(HWND wnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
}

void Player::NewFrame()
{
}

void Player::OnRender(float rgba[4])
{
}

void Player::OnRenderScene(ID3D11DeviceContext* pContext)
{
}

void Player::OnUpdate(float deltaTime)
{
}

void Player::ClearViewport(float rgba[4])
{
	game->ClearRenderTarget(rgba);
}

void Player::ResizeViewport(int w, int h)
{
}

void Player::HandleInput(float deltaTime)
{
}

LRESULT Player::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}
