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


void SceneHierarchy::OnDraw(bool* closable)
{
	ImGui::Begin(windowName);
	ImGui::Text("Scene Hierarchy:");
	
	EntityRegistrar::GetRegistry().each([&](auto entity)
		{
			EntityName& name = EntityRegistrar::GetRegistry().get<EntityName>(entity);

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
