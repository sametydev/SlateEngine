#pragma once
#include <TestEngine/Engine/DXConfig.h>
#include <d2d1.h>
#include <dwrite.h>

class D2DContext
{
public:
	D2DContext();
	~D2DContext();

	bool CreateDevices();
	void BeginResize();
	void OnResize();


	void OnRender();

	static D2DContext* Instance;
	
private:
	ComPtr<ID2D1Factory> m_d2dFactory;
	ComPtr<ID2D1RenderTarget> m_d2dRenderTarget;
	ComPtr<IDWriteFactory> m_d2dWriteFactory;

	ComPtr<ID2D1SolidColorBrush> m_solidColorBrush;
	ComPtr<IDWriteFont> m_writeFont;
	ComPtr<IDWriteTextFormat> m_textFormat;
};

