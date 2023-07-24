#include <TestEngine/Engine/Graphics/2D/Components/C2DText.h>
#include <TestEngine/Engine/Graphics/2D/D2DContext.h>

C2DText::C2DText()
{
}

C2DText::C2DText(LPCWSTR text, float x, float y, float w, float h) : m_text(text),m_rect{x,y,w,h}
{
	D2DContext::Instance->AddTextForRender(this);
}

C2DText::~C2DText()
{
}

void C2DText::OnRender()
{
	D2DContext::Instance->GetRenderTarget()->DrawTextW(m_text, wcslen(m_text), D2DContext::Instance->GetTextFormat().Get(),
		m_rect, D2DContext::Instance->GetSolidColorBrush().Get());
}
