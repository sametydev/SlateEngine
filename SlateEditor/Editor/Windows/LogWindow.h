#pragma once
#include <SlateEditor/Editor/Windows/IWindow.h>
#include <SlateEngine/Engine/Core/ILogger.h>
class LogWindow :
    public IWindow,public ILogger
{
public:
	LogWindow();
	~LogWindow();

	void OnInit() override;
	void OnDraw(bool* closable = nullptr) override;
	void AddLog(const char* fmt) override;
	void ClearLogs();

	static LogWindow* Instance;

private:
	ImGuiTextBuffer     m_buffer;
	ImGuiTextFilter     m_filter;
	ImVector<int>       m_lineOffset;
	bool                b_scrollBottom;
	const char* windowName;
};

