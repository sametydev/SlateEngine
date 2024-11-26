#pragma once
#include <SlateEngine/Engine/DXConfig.h>
class ENGINE_API EditorSystem
{
public:
	EditorSystem() {};
	virtual ~EditorSystem() {};

	virtual void OnInit(HWND wnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) = 0;

	virtual void NewFrame() = 0;
	virtual void OnRender() = 0;

	//Update first render later
	virtual void OnUpdate(float deltaTime) = 0;

	virtual void ClearViewport(float rgba[4]) = 0;

	virtual void ResizeViewport(int w, int h) = 0;

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

};

