#pragma once
#include <SlateEngine/Engine/Core/EnginePlayer.h>
#include <SlateEngine/Engine/Game/Game.h>

class Player : public EnginePlayer
{
public:
	Player();
	~Player();

	void OnInit(HWND wnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) override;

	void NewFrame() override;
	void OnRender(float rgba[4]) override;
	void OnRenderScene(ID3D11DeviceContext* pContext) override;

	//Update first render later
	void OnUpdate(float deltaTime) override;

	void ClearViewport(float rgba[4]) override;

	void ResizeViewport(int w, int h) override;
	void HandleInput(float deltaTime);

	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;


	static Player* Instance;

	Game* game = nullptr;
};

