#include <SlateEditor/Editor/Windows/InspectorWindow.h>
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include <SlateEditor/Editor/Windows/LogWindow.h>
#include <SlateEngine/Engine/Graphics/2D/Components/C2DText.h>
#include <SlateEngine/Engine/Graphics/BuiltInMesh.h>
#include <imgui_internal.h>

InspectorWindow::InspectorWindow()
{
}

InspectorWindow::~InspectorWindow()
{
}

void InspectorWindow::OnInit()
{
    windowName = "Inspector";
    game = Game::Instance;
}


void InspectorWindow::OnDraw(bool* closable)
{
}

void InspectorWindow::OnDraw(Entity* entity)
{
	if (ImGui::Begin("Inspector"))
	{
		if (entity != nullptr) {

			Transform& t = entity->GetComponent<Transform>();
			EntityName& e = entity->GetComponent<EntityName>();

			char* bufpass = _strdup(e.name);
			if (ImGui::InputText("Name", bufpass, 64)) {
				e.name = bufpass && !bufpass[0] ? "Entity" : bufpass;
			}

			/*
			=============================
			Draw Transform Rotation Scale
			=============================
			*/
			DrawTransform("Position", t.mPosition);
			DrawTransform("Rotation", t.mRotation);
			DrawTransform("Scale", t.mScale);

			/*
			======================
			Draw Components Button
			======================
			*/
			{
				ImGui::Dummy(ImVec2(0.0f, 30.0f));
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 70.f;
				ImVec2 buttonSize = { lineHeight + 3.0f, 25 };
				ImGui::Button("Components", buttonSize);
				ImGui::PopStyleColor(3);
				ImGui::Dummy(ImVec2(0.0f, 4.0f));
			}


			if (entity->HasComponent<RenderableGeometry>())
			{
				DrawRenderableGeometryComponent(entity);
			}
			if (entity->HasComponent<LuaScript>())
			{
				DrawLuaScriptComponent(entity);
			}


			/*
			==============
			Add Component
			==============
			*/
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				if (ImGui::Button("Add Component")) {
					ImGui::OpenPopup("AddComponent");
				}
				ImGui::PopStyleColor(1);

				if (ImGui::BeginPopup("AddComponent"))
				{
					if (ImGui::MenuItem("Renderable Component"))
					{
						if (!entity->HasComponent<RenderableGeometry>()) { 
							entity->AddComponent<RenderableGeometry>();
						}

						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

		}
	}
	ImGui::End();
}

void InspectorWindow::DrawTransform(const char* label, vec3f& val)
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::PushID(label);
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 80.f);
	ImGui::Dummy(ImVec2(0.0f, 2.0f));
	ImGui::Text(label);
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.5f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::Button("X", buttonSize);
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	ImGui::DragFloat("##X", &val.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::Button("Y", buttonSize);
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	ImGui::DragFloat("##Y", &val.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::Button("Z", buttonSize);
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	ImGui::DragFloat("##Z", &val.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::PopStyleVar();
	ImGui::Columns(1);
	ImGui::PopID();
}

void InspectorWindow::DrawRenderableGeometryComponent(Entity* entity)
{
	RenderableGeometry& r = entity->GetComponent<RenderableGeometry>();

	static int curr_mesh_item = 0;
	const char* mesh_strs[] = {
		"Box",
		"Sphere",
		"Cylinder"
	};

	if (ImGui::TreeNode("Renderable Geometry"))
	{
		ImGui::Dummy(ImVec2(0.0f, 4.0f));
		ImGui::Text("Select Built-In Geometry");
		if (ImGui::Combo("Mesh", &curr_mesh_item, mesh_strs, ARRAYSIZE(mesh_strs)))
		{
			MeshData<VertexPNT> meshData;
			switch (curr_mesh_item)
			{
				case 0: meshData = BuiltInMesh::CreateBox<VertexPNT>(); break;
				case 1: meshData = BuiltInMesh::CreateSphere<VertexPNT>(); break;
				case 2: meshData = BuiltInMesh::CreateCylinder<VertexPNT>(); break;
			}

			r.SetBuffer(meshData);
		}
		if (ImGui::Button("Remove <RenderableGeometry> Component")) {
			entity->RemoveComponent<RenderableGeometry>();
		}
		ImGui::Dummy(ImVec2(0.0f, 4.0f));
		ImGui::TreePop();
	}

	ImGui::Dummy(ImVec2(0.0f, 4.0f));

	if (ImGui::TreeNode("Material"))
	{
		ImGui::PushID(3);
		MaterialComponent& objMat = r.GetMaterial();

		for (auto& p : objMat.GetPropMap()) {
			switch (objMat.GetType(p.first))
			{
				case SVEC4:
					ImGui::ColorEdit3(p.first.data(), &std::get<vec4f>(p.second).x);
					break;
				case SVEC3:
					ImGui::ColorEdit3(p.first.data(), &std::get<vec3f>(p.second).x);
					break;
			}
		}

		ImGui::PopID();
		ImGui::TreePop();
	}
	ImGui::Dummy(ImVec2(0.0f, 4.0f));
}


void InspectorWindow::DrawLuaScriptComponent(Entity* entity)
{
	LuaScript& ls = entity->GetComponent<LuaScript>();

	static int currentScriptItem = 0;

	scriptsMapBuffer.clear();
	scriptNames.clear();
	scriptNamePtrs.clear();

	SlateFileSystemContainer* container = FileSystem::Instance->GetFileSystemContainer();
	for (auto& x : container->leafs[FILE_TYPE::LUA])
	{
		scriptNames.push_back(x.second.path);
		scriptNamePtrs.push_back(scriptNames.back().c_str());
		scriptsMapBuffer[x.second.path] = x.second.uuid;
	}

	if (ImGui::TreeNode("Script"))
	{
		ImGui::Dummy(ImVec2(0.0f, 4.0f));
		
		ImGui::Text(ls.GetScriptPath().c_str());
		if (ImGui::Combo("Mesh", &currentScriptItem, scriptNamePtrs.data(), scriptNamePtrs.size())) {
			//DXApplication::Instance->GetLogger()->AddLog(tempData[currentScriptItem]);
			ls.SetScriptByPath(scriptNamePtrs[currentScriptItem]);
		}

		if (ImGui::Button("Remove <Script> Component")) {
			entity->RemoveComponent<LuaScript>();
		}
		ImGui::Dummy(ImVec2(0.0f, 4.0f));
		ImGui::TreePop();
	}
	ImGui::Dummy(ImVec2(0.0f, 4.0f));

}


