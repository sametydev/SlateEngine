#include <SlateEngine/Engine/Entity/EntityManager.h>

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

void EntityManager::AddEntity(Entity* entity)
{
	entity->entity = entityRegistar.create();
	entityRegistar.emplace<EntityName>(entity->entity,"Entity");
}

void EntityManager::AddEntity(Entity* entity, const char* name)
{
	entity->entity = entityRegistar.create();
	entityRegistar.emplace<EntityName>(entity->entity, name);
}

void EntityManager::OnUpdate(float dt)
{
	auto view = entityRegistar.view<RenderableObject>();
	for (auto entity : view)
	{
		auto renderableObject = view.get<RenderableObject>(entity);

		renderableObject.OnUpdate(dt);
	}
}

void EntityManager::OnRender()
{
	auto view = entityRegistar.view<RenderableObject>();
	for (auto entity : view)
	{
		auto renderableObject = view.get<RenderableObject>(entity);

		renderableObject.OnRender();
	}
}
