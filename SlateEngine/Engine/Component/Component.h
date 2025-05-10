#pragma once
#include <SlateEngine/Engine/Entity/Entity.h>

using ComponentProperty = std::variant
<
	int,
	uint32_t, float,
	vec2f, vec3f, vec4f,
	mat4x4,
	std::vector<float>,
	std::vector<vec4f>,
	std::vector<mat4x4>,
	std::string
>;

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
		return m_connectedEntity;
	};

	inline void SetEntity(Entity* entity) {
		m_connectedEntity = entity;
		if (entity != nullptr) OnInternalInit();
	}

private:
	Entity* m_connectedEntity = nullptr;
};