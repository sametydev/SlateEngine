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

struct EntityName {
public:
	EntityName() = default;
	EntityName(std::string n, Entity* e) : name(n), entityClass(e) {};
	std::string name;
	Entity* entityClass = nullptr;

	inline void SetEntity(Entity* entity) {
		entityClass = entity;
	};
};