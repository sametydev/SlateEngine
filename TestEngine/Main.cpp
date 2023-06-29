#include <stdio.h>
#include <iostream>
#include <ostream>
#include <windows.h>
#include <TestEngine/DXConfig.h>
using namespace std;


int main() {
	UINT _createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel[] = {
		D3D_FEATURE_LEVEL_11_1
		};

	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dContext;


	HRESULT hr = D3D11CreateDevice(
		NULL, D3D_DRIVER_TYPE_HARDWARE,NULL,
		_createDeviceFlags,NULL,NULL,D3D11_SDK_VERSION,
		&m_d3dDevice,
		featureLevel,
		&m_d3dContext);

	if (FAILED(hr))
	{
		std::cout << "Creating of DirectX Device is Failed!" << std::endl;
		return false;
	}

	UINT m4xMsaaQuality;
	HR(m_d3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	bool mIsMsaaEnable = m4xMsaaQuality > 0;

	//Swapchain;

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



	IDXGIDevice* dxgiDevice = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIFactory1* dxgiFactory1 = nullptr;
	IDXGIFactory2* dxgiFactory2 = nullptr;

	HR(m_d3dDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));
	HR(dxgiDevice->GetAdapter(&dxgiAdapter));
	HR(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory1)));

	HR(dxgiFactory1->QueryInterface(IID_PPV_ARGS(&dxgiFactory2)));
	
	IDXGISwapChain1* mSwapChain;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fd{};
	fd.RefreshRate.Numerator = 60;
	fd.RefreshRate.Denominator = 1;
	fd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	fd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	fd.Windowed = TRUE;

	//Not Working Yet!
	HR(dxgiFactory2->CreateSwapChainForHwnd(m_d3dDevice, nullptr, &sd, &fd, nullptr,&mSwapChain));

	SAFE_RELEASE(dxgiFactory2);
	SAFE_RELEASE(dxgiFactory1);
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiDevice);


	//Creating RTV
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer));
	HR(m_d3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	SAFE_RELEASE(backBuffer);

	

	std::cout << "TestEngine Init" << "\n";

	return -1;
}