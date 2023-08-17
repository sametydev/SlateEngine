#pragma once
#include <entt.hpp>
#include <SlateEngine/Engine/Component/RenderableObject.h>

class Entity
{
public:
	Entity();
	~Entity();

	template<typename T>
	void AddComponent();

	template<typename T>
	T& GetComponent();

private:
	friend class EntityManager;
	entt::entity entity;
};

template<typename T>
inline void Entity::AddComponent()
{
	if (typeid(T) == typeid(RenderableObject)) {
		EntityManager::Instance->GetRegistar().emplace<T>(entity);
	}
}

template<typename T>
inline T& Entity::GetComponent()
{
	return EntityManager::Instance->GetRegistar().get<T>(entity);
}
