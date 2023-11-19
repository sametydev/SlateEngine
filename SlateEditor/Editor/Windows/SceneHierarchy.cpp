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

#define EntityRegistar EntityManager::Instance->GetRegistar()

void SceneHierarchy::OnDraw()
{
	ImGui::Begin(windowName);
	ImGui::Text("Scene Hierarchy:");

	EntityRegistar.each([&](auto entity)
		{
			EntityName& name = EntityRegistar.get<EntityName>(entity);

			bool isSelected = false;

			if (!(selectedEntity == nullptr)) {
				isSelected = (selectedEntity->rawEntity == entity);
			}

			ImGuiTreeNodeFlags flags = (isSelected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

			if (ImGui::TreeNodeEx(name.name, flags))
			{
				ImGui::TreePop();
			}

			if (ImGui::IsItemClicked())
			{
				selectedEntity = name.entityClass;
			}

		});

	ImGui::End();
}
