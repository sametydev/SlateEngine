#include "SceneHierarchy.h"

SceneHierarchy::SceneHierarchy()
{
}

SceneHierarchy::~SceneHierarchy()
{
}

void SceneHierarchy::OnInit()
{
	windowName = "Scene";
}

void SceneHierarchy::OnDraw(const char* title)
{
}

void SceneHierarchy::OnDraw()
{
	ImGui::Begin(windowName);
	ImGui::Text("Scene Hierarchy:");

	EntityManager::Instance->GetRegistar().each([](auto entity)
	{
			EntityName& name = EntityManager::Instance->GetRegistar().get<EntityName>(entity);
			if (ImGui::TreeNode(name.name))
			{
				ImGui::TreePop();
			}
	});

	ImGui::End();
}
