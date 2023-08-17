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

void EntityManager::OnAwake()
{

}

entt::entity EntityManager::CreateEntity()
{
	entt::entity e = entityRegistar.create();

	return e;
}
