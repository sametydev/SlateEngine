#include "NativeScriptingDebugger.h"

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
			std::vector<std::string> scripts = ScriptRegistry::Instance->GetRegisteredScripts();
			for (const auto& scriptName : scripts) {
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
				flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
				if (ImGui::TreeNodeEx(scriptName.c_str()))
				{
					ImGui::TreePop();
				}
			}

		}ImGui::End();
	}
}
