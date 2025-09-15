#pragma once
#include <SlateEditor/Editor/Windows/IWindow.h>

class ProfilerWindow :
	public IWindow
{
public:
	ProfilerWindow();
	~ProfilerWindow();

	void OnInit() override;
	void OnDraw(bool* closable = nullptr) override;
private:
	const char* windowName;
};
