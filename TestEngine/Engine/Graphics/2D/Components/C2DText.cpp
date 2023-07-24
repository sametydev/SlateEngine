#include <TestEngine/Engine/Graphics/2D/Components/C2DText.h>
#include <TestEngine/Engine/Graphics/2D/D2DContext.h>

C2DText::C2DText()
{
}

C2DText::C2DText(LPCWSTR text) : m_text(text)
{
	D2DContext::Instance->AddTextForRender(this);
}

C2DText::~C2DText()
{
}

void C2DText::OnRender()
{
	D2DContext::Instance->GetRenderTarget()->DrawTextW(m_text, wcslen(m_text), D2DContext::Instance->GetTextFormat().Get(),
		D2D1_RECT_F{ 0.0f, 0.0f, 600.0f, 200.0f }, D2DContext::Instance->GetSolidColorBrush().Get());
}
