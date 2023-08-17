#pragma once
#include <entt.hpp>

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	void OnAwake();

	entt::entity CreateEntity();

	static EntityManager* Instance;

private:
	entt::registry entityRegistar;
	
};

