#pragma once
#include <SlateEngine/Engine/Entity/Entity.h>

//Entity Component Signal Command
enum ECSignalCommand {
	ON_INIT,
	ON_UPDATE,
	ON_DESTROY,
	ON_SCENE_CHANGED,
	ON_EDITOR_LOAD
};

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	void RegisterEntity(Entity* entity);
	void RegisterEntity(Entity* entity,const char* name);

	Entity* GetEntityFromRaw(entt::entity e);

	void OnUpdate(float dt, int gameState);
	void OnRender(ID3D11DeviceContext* pDeviceContext);

	entt::registry& GetRegistar() { return entityRegistar; };

	void SendSignalToComponents(ECSignalCommand cmd);

	static EntityManager* Instance;

};



