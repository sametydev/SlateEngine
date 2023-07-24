#pragma once
#include <TestEngine/Engine/DXConfig.h>

class C2DText
{
public:
	C2DText();
	C2DText(LPCWSTR text);
	~C2DText();

	void OnRender();
private:
	LPCWSTR m_text;
};

