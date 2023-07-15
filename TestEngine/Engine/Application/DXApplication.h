#pragma once
#include <stdio.h>
#include <iostream>
#include <ostream>
#include <windows.h>
#include <TestEngine/DXConfig.h>
#include <TestEngine/Engine/Core/Timer.h>

class DXApplication
{
public:
	DXApplication(HINSTANCE hInstance);
	virtual ~DXApplication();

	void OnRun();

	virtual bool OnInit();
	virtual void OnResize();

	//Todo: Scene->OnUpdate
	virtual void OnUpdateScene(float dt) = 0;

	//Todo : OnRenderScene(Scene* scene)
	virtual void OnRenderScene() = 0;

	virtual LRESULT MsgProc(HWND hwnd, UINT msg,
		WPARAM wParam, LPARAM lParam);

	HINSTANCE GetApplicationInstance()const;
	HWND GetMainHWND() const;
	float GetAspectRatio() const;

protected:
	bool InitializeWindow();
	bool InitializeGraphics();

protected:
	HINSTANCE m_hInstance;
	HWND m_hwnd; 

	UINT m_msaaQuality; 
	bool m_msaaEnable = 0;

	Timer m_timer;

	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dContext;
	IDXGISwapChain* m_swapChain;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_vp;
	
	int m_clientw;
	int m_clienth;
};
