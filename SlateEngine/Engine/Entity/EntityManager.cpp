#include <SlateEngine/Engine/Entity/EntityManager.h>
#include <SlateEngine/Engine/Component/RenderableGeometry.h>
#include <SlateEngine/Engine/Component/LuaScript.h>
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

void EntityManager::OnUpdate(float dt,int gameState)
{

	auto transforms = entityRegistar.view<Transform>();
	for (auto entity : entityRegistar.view<Transform>())
	{
		transforms.get<Transform>(entity).OnUpdate(dt);
	}

	if (gameState == GameState::PLAYING)
	{
		auto scripts = entityRegistar.view<LuaScript>();
		for (auto entity : entityRegistar.view<LuaScript>())
		{
			scripts.get<LuaScript>(entity).OnUpdate(dt);
		}
	}


	auto RenderableGeometrys = entityRegistar.view<RenderableGeometry>();
	for (auto entity : RenderableGeometrys)
	{
		RenderableGeometrys.get<RenderableGeometry>(entity).OnUpdate(dt);
	}

}

void EntityManager::OnRender()
{
	auto RenderableGeometrys = entityRegistar.view<RenderableGeometry>();
	
	for (auto entity : RenderableGeometrys)
	{
		RenderableGeometrys.get<RenderableGeometry>(entity).OnRender();
	}
}
