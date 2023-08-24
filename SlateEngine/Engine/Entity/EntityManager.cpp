#include <SlateEngine/Engine/Entity/EntityManager.h>
#include <SlateEngine/Engine/Component/RenderableObject.h>

EntityManager* EntityManager::Instance = nullptr;

EntityManager::EntityManager()
{
	if (Instance == nullptr)
	{
		Instance = this;
	}
}

EntityManager::~EntityManager()
{
}

void EntityManager::RegisterEntity(Entity* entity)
{
	entity->rawEntity = entityRegistar.create();
	entityRegistar.emplace<EntityName>(entity->rawEntity,"Entity",entity);
	entityRegistar.emplace<Transform>(entity->rawEntity);
}

void EntityManager::RegisterEntity(Entity* entity, const char* name)
{
	entity->rawEntity = entityRegistar.create();
	entityRegistar.emplace<EntityName>(entity->rawEntity, name,entity);
	entityRegistar.emplace<Transform>(entity->rawEntity);
}

Entity* EntityManager::GetEntityFromRaw(entt::entity e)
{
	EntityName& en = entityRegistar.get<EntityName>(e);

	return en.entityClass;
}

void EntityManager::OnUpdate(float dt,int GameState)
{
	if (GameState != 1) return;

	auto transforms = entityRegistar.view<Transform>();
	for (auto entity : transforms)
	{
		transforms.get<Transform>(entity).OnUpdate(dt);
	}

	auto renderableObjects = entityRegistar.view<RenderableObject>();
	for (auto entity : renderableObjects)
	{
		renderableObjects.get<RenderableObject>(entity).OnUpdate(dt);
	}

}

void EntityManager::OnRender()
{
	auto renderableObjects = entityRegistar.view<RenderableObject>();
	for (auto entity : renderableObjects)
	{
		renderableObjects.get<RenderableObject>(entity).OnRender();
	}
}
