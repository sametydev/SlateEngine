#pragma once
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Graphics/2D/Components/C2DText.h>

#include <d2d1.h>
#include <dwrite.h>
#include <set>

#include <entt.hpp>

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

	entt::registry& GetRegistar() { return r2registar; };

	void AddTextForRender(LPCWSTR t, float x, float y, float w, float h);
private:
	friend class UIRenderablesWindow;
	ComPtr<ID2D1Factory> m_d2dFactory;
	ComPtr<ID2D1RenderTarget> m_d2dRenderTarget;
	ComPtr<IDWriteFactory> m_d2dWriteFactory;

	ComPtr<ID2D1SolidColorBrush> m_solidColorBrush;
	ComPtr<IDWriteFont> m_writeFont;
	ComPtr<IDWriteTextFormat> m_textFormat;

	entt::registry r2registar;
};

