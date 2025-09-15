#include "NativeScriptingDebugger.h"
#include <SlateEngine\Engine\Core\ProfileTimerFactory.h>

NativeScriptingDebugger::NativeScriptingDebugger()
{
}

NativeScriptingDebugger::~NativeScriptingDebugger()
{
}

void NativeScriptingDebugger::OnInit()
{
}

void NativeScriptingDebugger::OnDraw(bool* closable)
{
	if (*closable)
	{
		if (ImGui::Begin("Native Scripting Debugger", closable)) {
			ImGui::Text("List of imported classes:");
			for (const auto& scriptName : ScriptRegistry::Instance->GetRegisteredScripts()) {

				if (ImGui::TreeNodeEx(scriptName.c_str(), 0))ImGui::TreePop();
			}

			ImGui::Text("If you cannot see your script/class here,\nplease register from DLL,\nfor more information please look Documentation");
		}ImGui::End();
	}
}
