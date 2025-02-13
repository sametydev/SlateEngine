#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <wrl.h>

class ProjectHub
{
public:
	ProjectHub();
	~ProjectHub();
	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
	void RenderPanel();
	int OnRender();
	int Init();

	static ProjectHub* Instance;

	UINT						g_ResizeWidth = 0, g_ResizeHeight = 0;
private:
	Microsoft::WRL::ComPtr<ID3D11Device>				p_d3dDevice = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			p_d3dDeviceContext = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain>				p_SwapChain = nullptr;
	bool												swapChainOccluded = false;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		mainRenderTargetView = nullptr;

	HWND												mHWND = 0;
};