#pragma once
#include <TestEngine/Engine/DXConfig.h>
#include <TestEngine/Engine/Graphics/2D/Components/C2DText.h>

#include <d2d1.h>
#include <dwrite.h>
#include <set>

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

	ComPtr<ID2D1RenderTarget> GetRenderTarget() {
		return m_d2dRenderTarget;
	}
	ComPtr<ID2D1SolidColorBrush> GetSolidColorBrush() {
		return m_solidColorBrush;
	}
	ComPtr<IDWriteTextFormat> GetTextFormat() {
		return m_textFormat;
	}

	void AddTextForRender(C2DText* text);
private:
	friend class UIRenderablesWindow;
	ComPtr<ID2D1Factory> m_d2dFactory;
	ComPtr<ID2D1RenderTarget> m_d2dRenderTarget;
	ComPtr<IDWriteFactory> m_d2dWriteFactory;

	ComPtr<ID2D1SolidColorBrush> m_solidColorBrush;
	ComPtr<IDWriteFont> m_writeFont;
	ComPtr<IDWriteTextFormat> m_textFormat;

	std::set<C2DText*> m_texts;
};

