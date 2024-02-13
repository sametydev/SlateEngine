#pragma once
#include <entt.hpp>

extern entt::registry entityRegistar;

class Entity
{
public:
	Entity() = default;
	~Entity() = delete;

	template<typename T>
	void AddComponent();

	template<typename T>
	void RemoveComponent();

	template<typename T>
	bool HasComponent();

	template<typename T>
	T& GetComponent();


	entt::entity rawEntity;

private:
	friend class EntityManager;
};

template<typename T>
inline void Entity::AddComponent()
{
	if (HasComponent<T>()) return;
	entityRegistar.emplace<T>(rawEntity).SetEntity(this);
}

template<typename T>
inline void Entity::RemoveComponent()
{
	entityRegistar.erase<T>(rawEntity);
}

template<typename T>
inline bool Entity::HasComponent()
{
	return entityRegistar.any_of<T>(rawEntity);
}

template<typename T>
inline T& Entity::GetComponent()
{
	return entityRegistar.get<T>(rawEntity);
}

struct EntityName {
public:
	EntityName(const char* n, Entity* e) : name(n), entityClass(e) {};
	const char* name;
	Entity* entityClass = nullptr;
};