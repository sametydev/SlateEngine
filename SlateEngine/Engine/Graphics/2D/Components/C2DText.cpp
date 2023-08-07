#include <SlateEngine/Engine/Graphics/2D/Components/C2DText.h>
#include <SlateEngine/Engine/Graphics/2D/D2DContext.h>

C2DText::C2DText()
{
}

C2DText::C2DText(LPCWSTR text, float x, float y, float w, float h) : m_text(text),m_rect{x,y,w,h}
{
	D2DContext::Instance->AddTextForRender(this);

	char* resultStr = new char[wcslen(text) + 1];
	wsprintfA(resultStr, "%S", text);
	m_textc = resultStr;
}

C2DText::~C2DText()
{
	//D2DContext::Instance->RemoveTextForRender(this);
}

void C2DText::OnRender()
{
	D2DContext::Instance->GetRenderTarget()->DrawTextW(m_text, wcslen(m_text), D2DContext::Instance->GetTextFormat().Get(),
		m_rect, D2DContext::Instance->GetSolidColorBrush().Get());
}

const char* C2DText::GetText()
{
	return m_textc;
}
