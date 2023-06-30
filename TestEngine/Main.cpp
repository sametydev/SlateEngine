#include <stdio.h>
#include <iostream>
#include <ostream>
#include <windows.h>
#include <TestEngine/DXConfig.h>
using namespace std;
#pragma warning (disable:26495)

LRESULT CALLBACK WndProc(HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (msg)
	{

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			if (MessageBox(0, L"Are you sure you want to exit?",
				L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
				DestroyWindow(hwnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}
HWND hwnd;

bool InitializeWindow(HINSTANCE hInstance,
	int ShowWnd,
	int width, int height,
	bool fullscreen)
{
	if (fullscreen)
	{
		HMONITOR hmon = MonitorFromWindow(hwnd,
			MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(hmon, &mi);

		width = mi.rcMonitor.right - mi.rcMonitor.left;
		height = mi.rcMonitor.bottom - mi.rcMonitor.top;
	}

	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"Main";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	hwnd = CreateWindowEx(NULL,
		L"Main",
		L"Main",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hwnd)
	{
		MessageBox(NULL, L"Error creating window",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}
	if (fullscreen)
	{
		SetWindowLong(hwnd, GWL_STYLE, 0);
	}

	ShowWindow(hwnd, ShowWnd);
	UpdateWindow(hwnd);

	return true;
}


int main() {
	InitializeWindow(0, 1, 800, 600, 0);
	MSG msg{};


	///
	/// TODO:
	/// Cleanup and organize code
	/// 
	
	//for dxDebugging
	UINT _createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//for using DirectX 11.1, Its diffrent than DirectX 11.0
	D3D_FEATURE_LEVEL featureLevel[] = {
		D3D_FEATURE_LEVEL_11_1
		};


	//Creating Device and Context variables
	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dContext;


	//Creating D3D Device
	HRESULT hr = D3D11CreateDevice(
		NULL, D3D_DRIVER_TYPE_HARDWARE,NULL,
		_createDeviceFlags,NULL,NULL,D3D11_SDK_VERSION,
		&m_d3dDevice,
		featureLevel,
		&m_d3dContext);

	//Check some errors
	if (FAILED(hr))
	{
		std::cout << "Creating of DirectX Device is Failed!" << std::endl;
		return false;
	}

	//Check MSAA
	UINT m4xMsaaQuality;
	HR(m_d3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	bool mIsMsaaEnable = m4xMsaaQuality > 0;

	//Swapchain1 (Its diffrent than Swapchain0 0 for DX11_0, 1 is for DX11_1);

	DXGI_SWAP_CHAIN_DESC1 sd{};
	sd.Width = 800;
	sd.Height = 600;
	sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = mIsMsaaEnable ? 4 : 1;
	sd.SampleDesc.Quality = mIsMsaaEnable ? m4xMsaaQuality - 1 : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.Flags = 0;
	sd.Scaling = DXGI_SCALING_STRETCH;
	sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;


	//Creating adapter and factory variables
	IDXGIDevice* dxgiDevice = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIFactory1* dxgiFactory1 = nullptr;
	IDXGIFactory2* dxgiFactory2 = nullptr;

	//Attaching adapter and factories
	HR(m_d3dDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));
	HR(dxgiDevice->GetAdapter(&dxgiAdapter));
	HR(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory1)));
	HR(dxgiFactory1->QueryInterface(IID_PPV_ARGS(&dxgiFactory2)));
	
	//Creating swapchain1 object
	IDXGISwapChain1* mSwapChain;

	//Creating descriptor
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fd{};
	fd.RefreshRate.Numerator = 60;
	fd.RefreshRate.Denominator = 1;
	fd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	fd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	fd.Windowed = TRUE;

	//Not Working Yet! Because We dont have a Window Class
	HR(dxgiFactory2->CreateSwapChainForHwnd(m_d3dDevice, hwnd, &sd, &fd, nullptr,&mSwapChain));

	SAFE_RELEASE(dxgiFactory2);
	SAFE_RELEASE(dxgiFactory1);
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiDevice);


	//Creating RTV (RenderTarget View)
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer));
	HR(m_d3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	
	SAFE_RELEASE(backBuffer);

	std::cout << "TestEngine Init" << "\n";

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = 800;
	depthStencilDesc.Height = 600;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (mIsMsaaEnable)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;

	HR(m_d3dDevice->CreateTexture2D(
		&depthStencilDesc,
		0,
		&mDepthStencilBuffer));
	HR(m_d3dDevice->CreateDepthStencilView(
		mDepthStencilBuffer,
		0,
		&mDepthStencilView));


	m_d3dContext->OMSetRenderTargets(
		1, &mRenderTargetView, mDepthStencilView);

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return -1;
}