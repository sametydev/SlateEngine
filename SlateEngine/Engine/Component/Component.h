#pragma once
#include <SlateEngine/Engine/Entity/Entity.h>

class ENGINE_API Component {
public:
	Component() {};

	virtual ~Component() {};
	virtual void OnInternalInit() = 0;
	virtual void OnInit() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnRender(ID3D11DeviceContext* pDeviceContext) = 0;
	virtual void OnShutdown() = 0;

	inline Entity* GetEntity() {
		return connectedEntity;
	};

	inline void SetEntity(Entity* entity) {
		connectedEntity = entity;
		if (entity != nullptr) OnInternalInit();
	}

private:
	Entity* connectedEntity = nullptr;
};