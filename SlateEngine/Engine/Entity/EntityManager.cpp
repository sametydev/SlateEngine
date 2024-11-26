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
	entity->rawEntity = EntityRegistrar::entityRegistar.create();
	EntityRegistrar::entityRegistar.emplace<EntityName>(entity->rawEntity,"Entity",entity);
	EntityRegistrar::entityRegistar.emplace<Transform>(entity->rawEntity);
}

void EntityManager::RegisterEntity(Entity* entity, const char* name)
{
	entity->rawEntity = EntityRegistrar::entityRegistar.create();
	EntityRegistrar::entityRegistar.emplace<EntityName>(entity->rawEntity, name,entity);
	EntityRegistrar::entityRegistar.emplace<Transform>(entity->rawEntity);
}

Entity* EntityManager::GetEntityFromRaw(entt::entity e)
{
	EntityName& en = EntityRegistrar::entityRegistar.get<EntityName>(e);

	return en.entityClass;
}

void EntityManager::OnUpdate(float dt,int gameState)
{

	auto transforms = EntityRegistrar::entityRegistar.view<Transform>();
	auto renderableGeometrys = EntityRegistrar::entityRegistar.view<RenderableGeometry>();

	for (auto entity : transforms, renderableGeometrys)
	{
		transforms.get<Transform>(entity).OnUpdate(dt);
		renderableGeometrys.get<RenderableGeometry>(entity).OnUpdate(dt);

	}

	if (gameState == GameState::PLAYING)
	{
		auto scripts = EntityRegistrar::entityRegistar.view<LuaScript>();
		for (auto entity : scripts)
		{
			scripts.get<LuaScript>(entity).OnUpdate(dt);
		}
	}

}

void EntityManager::OnRender(ID3D11DeviceContext* pDeviceContext)
{
	auto RenderableGeometrys = EntityRegistrar::entityRegistar.view<RenderableGeometry>();
	
	for (auto entity : RenderableGeometrys)
	{
		RenderableGeometrys.get<RenderableGeometry>(entity).OnRender(pDeviceContext);
	}
}

void EntityManager::SendSignalToComponents(ECSignalCommand cmd)
{
	auto transforms = EntityRegistrar::entityRegistar.view<Transform>();

	auto renderableGeometrys = EntityRegistrar::entityRegistar.view<RenderableGeometry>();
	auto materials = EntityRegistrar::entityRegistar.view<MaterialComponent>();
	auto scripts = EntityRegistrar::entityRegistar.view<LuaScript>();

	switch (cmd)
	{
	case ON_INIT:

			for (auto entity : transforms,renderableGeometrys,scripts)
			{
				transforms.get<Transform>(entity).OnInit();
				scripts.get<LuaScript>(entity).OnInit();
				renderableGeometrys.get<RenderableGeometry>(entity).OnInit();
				materials.get<MaterialComponent>(entity).OnInit();
			}

		break;
	case ON_DESTROY:
		break;
	case ON_SCENE_CHANGED:
		break;
	case ON_EDITOR_LOAD:
		break;
	default:
		break;
	}
}
