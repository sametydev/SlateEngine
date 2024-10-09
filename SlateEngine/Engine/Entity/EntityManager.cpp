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
	auto renderableGeometrys = entityRegistar.view<RenderableGeometry>();

	for (auto entity : transforms, renderableGeometrys)
	{
		transforms.get<Transform>(entity).OnUpdate(dt);
		renderableGeometrys.get<RenderableGeometry>(entity).OnUpdate(dt);

	}

	if (gameState == GameState::PLAYING)
	{
		auto scripts = entityRegistar.view<LuaScript>();
		for (auto entity : scripts)
		{
			scripts.get<LuaScript>(entity).OnUpdate(dt);
		}
	}

}

void EntityManager::OnRender(ID3D11DeviceContext* pDeviceContext)
{
	auto RenderableGeometrys = entityRegistar.view<RenderableGeometry>();
	
	for (auto entity : RenderableGeometrys)
	{
		RenderableGeometrys.get<RenderableGeometry>(entity).OnRender(pDeviceContext);
	}
}

void EntityManager::SendSignalToComponents(ECSignalCommand cmd)
{
	auto transforms = entityRegistar.view<Transform>();

	auto renderableGeometrys = entityRegistar.view<RenderableGeometry>();
	auto materials = entityRegistar.view<MaterialComponent>();

	auto scripts = entityRegistar.view<LuaScript>();

	switch (cmd)
	{
	case ON_INIT:

			for (auto entity : transforms,renderableGeometrys,scripts, materials)
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
