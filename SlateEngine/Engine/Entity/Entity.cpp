#include "Entity.h"
#include <SlateEngine/Engine/Component/RenderableGeometry.h>
#include <SlateEngine/Engine/Component/LuaScript.h>
template<typename T>
void Entity::AddComponent()
{
	if (HasComponent<T>()) return;
	EntityRegistrar::GetRegistry().emplace<T>(rawEntity).SetEntity(this);
}

template<typename T>
void Entity::AddComponentForce()
{
	EntityRegistrar::GetRegistry().emplace<T>(rawEntity).SetEntity(this);
}

template<typename T>
void Entity::RemoveComponent()
{
	EntityRegistrar::GetRegistry().erase<T>(rawEntity);
}

template<typename T>
bool Entity::HasComponent()
{
	return EntityRegistrar::GetRegistry().any_of<T>(rawEntity);
}

template<typename T>
T& Entity::GetComponent()
{
	return EntityRegistrar::GetRegistry().get<T>(rawEntity);
}
template<typename T>
T* Entity::GetComponentAsPointer()
{
	if (!HasComponent<T>()) return nullptr;
	return &EntityRegistrar::GetRegistry().get<T>(rawEntity);
}
#define INSTANTIATE_COMPONENTS_FOR_ENTITY(T) \
    template ENGINE_API  void Entity::AddComponent<T>(); \
    template ENGINE_API  void Entity::RemoveComponent<T>(); \
    template ENGINE_API  bool Entity::HasComponent<T>(); \
    template ENGINE_API  T& Entity::GetComponent<T>(); \
    template ENGINE_API  T* Entity::GetComponentAsPointer<T>();

INSTANTIATE_COMPONENTS_FOR_ENTITY(RenderableGeometry)
INSTANTIATE_COMPONENTS_FOR_ENTITY(Transform)
INSTANTIATE_COMPONENTS_FOR_ENTITY(MaterialComponent)
INSTANTIATE_COMPONENTS_FOR_ENTITY(LuaScript)
INSTANTIATE_COMPONENTS_FOR_ENTITY(EntityName)