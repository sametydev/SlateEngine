#pragma once
#include <SlateEditor/Editor/Windows/IWindow.h>
#include <SlateEngine/Engine/NativeScripting/ScriptRegistry.h>

class NativeScriptingDebugger :
	public IWindow
{
public:
	NativeScriptingDebugger();
	~NativeScriptingDebugger();

	void OnInit() override;
	void OnDraw(bool* closable = nullptr) override;
private:
	const char* windowName;
};
