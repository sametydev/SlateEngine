#pragma once
#include <entt.hpp>

extern entt::registry entityRegistar;

class Entity
{
public:
	Entity();
	~Entity();

	template<typename T>
	void AddComponent();

	template<typename T>
	T& GetComponent();


	entt::entity rawEntity;

private:
	friend class EntityManager;
};

template<typename T>
inline void Entity::AddComponent()
{
	entityRegistar.emplace<T>(rawEntity).SetEntity(this);
}

template<typename T>
inline T& Entity::GetComponent()
{
	return entityRegistar.get<T>(rawEntity);
}

struct EntityName {
public:
	EntityName(const char* n,Entity* e) : name(n), entityClass(e) {};
	const char* name;
	Entity* entityClass = nullptr;
};