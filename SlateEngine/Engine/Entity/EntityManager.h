#pragma once
#include <SlateEngine/Engine/Entity/Entity.h>

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	void RegisterEntity(Entity* entity);
	void RegisterEntity(Entity* entity,const char* name);

	Entity* GetEntityFromRaw(entt::entity e);

	void OnUpdate(float dt);
	void OnRender();

	entt::registry& GetRegistar() { return entityRegistar; };

	static EntityManager* Instance;
};



