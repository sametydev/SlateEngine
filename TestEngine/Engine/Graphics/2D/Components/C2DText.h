#pragma once
#include <TestEngine/Engine/DXConfig.h>
#include <d2d1.h>
#include <dwrite.h>

class C2DText
{
public:
	C2DText();
	C2DText(LPCWSTR text,float x, float y, float w, float h);
	~C2DText();

	void OnRender();
private:
	LPCWSTR m_text;
	D2D1_RECT_F m_rect;
};

