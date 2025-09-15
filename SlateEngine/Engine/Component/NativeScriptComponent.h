#pragma once
#include <SlateEngine/Engine/Component/Component.h>
#include <SlateEngine/Engine/Component/Script.h>
#include <SlateEngine/Engine/NativeScripting/ScriptRegistry.h>
#include <unordered_map>

class NativeScriptComponent : public Component
{
public:
	NativeScriptComponent();
	void OnInternalInit() override;
	void OnInit() override;
	void OnUpdate(float deltaTime) override;
	void OnRender(ID3D11DeviceContext* pDeviceContext) override;
	void OnShutdown() override;

	inline void AddScript(std::string name) {
		if (scripts.count(name)) return;
		if (!ScriptRegistry::Instance->Exists(name)) return;
		scripts[name] = ScriptRegistry::Instance->Create(name);
	}

	inline void RemoveScript(std::string name) {
		if (!ScriptRegistry::Instance->Exists(name)) return;
		if (!scripts.count(name)) return;
		Script* buffer = scripts[name];
		scripts.erase(name);
		delete buffer;
	}

private:
	std::unordered_map<std::string, Script*> scripts;
};

