#pragma once
#include <entt.hpp>
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Entity/EntityRegistrar.h>

class ENGINE_API Entity
{
public:
	Entity() = default;
	~Entity() = delete;

	template<typename T>
	void AddComponent();

	template<typename T>
	void AddComponentForce();

	template<typename T>
	void RemoveComponent();

	template<typename T>
	bool HasComponent();

	template<typename T>
	T& GetComponent();

	template<typename T>
	T* GetComponentAsPointer();

	entt::entity rawEntity;

private:
	friend class EntityManager;
};

template<typename T>
inline void Entity::AddComponent()
{
	if (HasComponent<T>()) return;
	EntityRegistrar::entityRegistar.emplace<T>(rawEntity).SetEntity(this);
}

template<typename T>
inline void Entity::AddComponentForce()
{
	EntityRegistrar::entityRegistar.emplace<T>(rawEntity).SetEntity(this);
}

template<typename T>
inline void Entity::RemoveComponent()
{
	EntityRegistrar::entityRegistar.erase<T>(rawEntity);
}

template<typename T>
inline bool Entity::HasComponent()
{
	return EntityRegistrar::entityRegistar.any_of<T>(rawEntity);
}

template<typename T>
inline T& Entity::GetComponent()
{
	return EntityRegistrar::entityRegistar.get<T>(rawEntity);
}
template<typename T>
inline T* Entity::GetComponentAsPointer()
{
	if (!HasComponent<T>()) return nullptr;
	return &EntityRegistrar::entityRegistar.get<T>(rawEntity);
}

struct EntityName {
public:
	EntityName(const char* n, Entity* e) : name(n), entityClass(e) {};
	const char* name;
	Entity* entityClass = nullptr;
};