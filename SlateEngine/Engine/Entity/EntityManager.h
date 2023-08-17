#pragma once
#include <SlateEngine/Engine/Entity/Entity.h>

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	void AddEntity(Entity* entity);

	void OnUpdate(float dt);
	void OnRender();

	entt::registry& GetRegistar() { return entityRegistar; };

	static EntityManager* Instance;

private:
	entt::registry entityRegistar;
};

