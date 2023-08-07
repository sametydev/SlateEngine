#pragma once
#include <SlateEngine/Engine/Editor/Windows/IWindow.h>

class LogWindow :
    public IWindow
{
public:
	LogWindow();
	~LogWindow();

	void OnInit() override;
	void OnDraw(const char* title) override;
	void OnDraw() override;
	void AddLog(const char* fmt,...);
	void ClearLogs();

	static LogWindow* Instance;

private:
	ImGuiTextBuffer     m_buffer;
	ImGuiTextFilter     m_filter;
	ImVector<int>       m_lineOffset;
	bool                b_scrollBottom;
	const char* windowName;
};

